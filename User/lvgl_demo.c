/**
 ****************************************************************************************************
 * @file        lvgl_demo.c
 * @author      ÕıµãÔ­×ÓÍÅ¶Ó(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8²Ù×÷ÏµÍ³ÒÆÖ²
 * @license     Copyright (c) 2020-2032, ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾
 ****************************************************************************************************
 * @attention
 *
 * ÊµÑéÆ½Ì¨:ÕıµãÔ­×Ó Ì½Ë÷Õß F407¿ª·¢°å
 * ÔÚÏßÊÓÆµ:www.yuanzige.com
 * ¼¼ÊõÂÛÌ³:www.openedv.com
 * ¹«Ë¾ÍøÖ·:www.alientek.com
 * ¹ºÂòµØÖ·:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "lvgl_demo.h"
#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demo_stress.h"
#include "./BSP/ADC/adc.h"

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

#include "arm_math.h"
#include "arm_const_structs.h"

SemaphoreHandle_t xSemaphore = NULL; // äº’æ–¥é”å¥æŸ„
/******************************************************************************************************/
/*FreeRTOSÅäÖÃ*/

/* START_TASK ÈÎÎñ ÅäÖÃ
 * °üÀ¨: ÈÎÎñ¾ä±ú ÈÎÎñÓÅÏÈ¼¶ ¶ÑÕ»´óĞ¡ ´´½¨ÈÎÎñ
 */
#define START_TASK_PRIO     1           /* ÈÎÎñÓÅÏÈ¼¶ */
#define START_STK_SIZE      128         /* ÈÎÎñ¶ÑÕ»´óĞ¡ */
TaskHandle_t StartTask_Handler;         /* ÈÎÎñ¾ä±ú */
void start_task(void *pvParameters);    /* ÈÎÎñº¯Êı */

/* LV_DEMO_TASK ÈÎÎñ ÅäÖÃ
 * °üÀ¨: ÈÎÎñ¾ä±ú ÈÎÎñÓÅÏÈ¼¶ ¶ÑÕ»´óĞ¡ ´´½¨ÈÎÎñ
 */
#define LV_DEMO_TASK_PRIO   3           /* ÈÎÎñÓÅÏÈ¼¶ */
#define LV_DEMO_STK_SIZE    1024        /* ÈÎÎñ¶ÑÕ»´óĞ¡ */
TaskHandle_t LV_DEMOTask_Handler;       /* ÈÎÎñ¾ä±ú */
void lv_demo_task(void *pvParameters);  /* ÈÎÎñº¯Êı */

#define  ADC1_DMA_Size	 1024      //ADCÊı¾İ¸öÊı
 
extern uint32_t ADC1_ConvertedValue[ ADC1_DMA_Size ];   //¶¨ÒåADCÊı¾İ´æ´¢Êı×é

#define  FFT_LENGTH		   1024
extern int SAM_FRE; //²ÉÑùÂÊ

/******************************************************************************************************/

/**
 * @brief       lvgl_demoÈë¿Úº¯Êı
 * @param       ÎŞ
 * @retval      ÎŞ
 */
void lvgl_demo(void)
{
    lv_init();                                          /* lvglÏµÍ³³õÊ¼»¯ */
    lv_port_disp_init();                                /* lvglÏÔÊ¾½Ó¿Ú³õÊ¼»¯,·ÅÔÚlv_init()µÄºóÃæ */
    lv_port_indev_init();                               /* lvglÊäÈë½Ó¿Ú³õÊ¼»¯,·ÅÔÚlv_init()µÄºóÃæ */

    xTaskCreate((TaskFunction_t )start_task,            /* ÈÎÎñº¯Êı */
                (const char*    )"start_task",          /* ÈÎÎñÃû³Æ */
                (uint16_t       )START_STK_SIZE,        /* ÈÎÎñ¶ÑÕ»´óĞ¡ */
                (void*          )NULL,                  /* ´«µİ¸øÈÎÎñº¯ÊıµÄ²ÎÊı */
                (UBaseType_t    )START_TASK_PRIO,       /* ÈÎÎñÓÅÏÈ¼¶ */
                (TaskHandle_t*  )&StartTask_Handler);   /* ÈÎÎñ¾ä±ú */

    vTaskStartScheduler();                              /* ¿ªÆôÈÎÎñµ÷¶È */
}

/**
 * @brief       start_task
 * @param       pvParameters : ´«Èë²ÎÊı(Î´ÓÃµ½)
 * @retval      ÎŞ
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    xSemaphore = xSemaphoreCreateMutex();
    taskENTER_CRITICAL();           /* ½øÈëÁÙ½çÇø */

    /* ´´½¨LVGLÈÎÎñ */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    xTaskCreate(adc_scan_task,
                "adc_scan_task",
                512, 
                NULL,
                3,
                NULL);
                
    taskEXIT_CRITICAL();            /* ÍË³öÁÙ½çÇø */
    vTaskDelete(StartTask_Handler); /* É¾³ı¿ªÊ¼ÈÎÎñ */
}

/**
 * @brief       LVGLÔËĞĞÀı³Ì
 * @param       pvParameters : ´«Èë²ÎÊı(Î´ÓÃµ½)
 * @retval      ÎŞ
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    while(1)
    {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        lv_timer_handler(); /* LVGLİ†Ê±Ç· */
		xSemaphoreGive(xSemaphore);
        // ÏÔÊ¾´¦Àí
        vTaskDelay(5);
    }
}


lv_obj_t * adc_label;
lv_obj_t* line1, *line2, *line3;
int pos = 0;
extern uint8_t  g_timxchy_cap_sta;              /* è¾“å…¥æ•è·çŠ¶æ€ */
extern uint16_t g_timxchy_cap_val;              /* è¾“å…¥æ•è·å€¼ */
lv_obj_t * ult_label;
float adc_arr[1024];
void adc_scan_task(void *arg){
	uint16_t adcx, adcx2; float temp;
    adc_label = lv_label_create(lv_scr_act());

	while(1){
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
//            adcx = adc_get_result_average(ADC_ADCX_CHY, 1);

//            temp = (float)adcx * (3.3 / 4096);            /* è·å–è®¡ç®—åçš„å¸¦å°æ•°çš„å®é™…ç”µå‹å€¼ï¼Œæ¯”å¦‚3.1111 */
//            adc_arr[i]=temp;
//            adcx = temp;
//            temp -= adcx;                                       /* æŠŠå·²ç»æ˜¾ç¤ºçš„æ•´æ•°éƒ¨åˆ†å»æ‰ï¼Œç•™ä¸‹å°æ•°éƒ¨åˆ†ï¼Œæ¯”å¦‚3.1111-3=0.1111 */
//            temp *= 1000; 
//            adcx2 = temp;
//            lv_label_set_text_fmt(adc_label, "ADC:%d.%dV", adcx, adcx2);
        HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC1_ConvertedValue,ADC1_DMA_Size);
      	  for(int i=0;i<FFT_LENGTH;i++)
	  {
			printf("%f\r\n",ADC1_ConvertedValue[ i ]*3.3f/4095.0f);			
	  }
       
        xSemaphoreGive(xSemaphore);

	}
}



