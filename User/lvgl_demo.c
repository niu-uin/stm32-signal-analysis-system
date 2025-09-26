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

/* LED_TASK ÈÎÎñ ÅäÖÃ
 * °üÀ¨: ÈÎÎñ¾ä±ú ÈÎÎñÓÅÏÈ¼¶ ¶ÑÕ»´óĞ¡ ´´½¨ÈÎÎñ
 */
#define LED_TASK_PRIO       4           /* ÈÎÎñÓÅÏÈ¼¶ */
#define LED_STK_SIZE        128         /* ÈÎÎñ¶ÑÕ»´óĞ¡ */
TaskHandle_t LEDTask_Handler;           /* ÈÎÎñ¾ä±ú */
void led_task(void *pvParameters);      /* ÈÎÎñº¯Êı */

int current_task=1;

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

    /* LED²âÊÔÈÎÎñ */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
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
    lv_scr_name();
//    lv_demo_stress();       /* ²âÊÔµÄdemo */
    while(1)
    {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        lv_timer_handler(); /* LVGLİ†Ê±Ç· */
		xSemaphoreGive(xSemaphore);
        change_scr();
        vTaskDelay(5);
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : ´«Èë²ÎÊı(Î´ÓÃµ½)
 * @retval      ÎŞ
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(1000);
    }
}

LV_FONT_DECLARE(name_font);
void lv_scr_name(){
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    lv_obj_clean(lv_scr_act());
    lv_obj_t *name_scr=lv_label_create(lv_scr_act());
    lv_obj_center(name_scr);
    lv_obj_set_style_text_font(name_scr, &name_font, 0);
    lv_label_set_text(name_scr,"ç§¦é¢–     1231002031\næ™¯è²è² 1231002027\nå´”å®‡æ¬£ 1231002025");
    xSemaphoreGive(xSemaphore);
}

void lv_scr_img()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    LV_IMG_DECLARE(img_pigg);
    lv_obj_clean(lv_scr_act());
    lv_obj_t *img_scr=lv_label_create(lv_scr_act());       
    lv_obj_t * img = lv_img_create(img_scr);
    lv_img_set_src(img, &img_pigg);
    lv_obj_set_size(img, 320, 240);
    xSemaphoreGive(xSemaphore);
}
lv_obj_t * adc_label;
lv_obj_t* line1, *line2, *line3;
static lv_point_t line_points[320] = { {0, 1}};
static lv_point_t line_v[2] = { {0, 0}};
static lv_point_t line_l[2] = { {0, 0}};
int pos = 0;
extern uint8_t  g_timxchy_cap_sta;              /* è¾“å…¥æ•è·çŠ¶æ€ */
extern uint16_t g_timxchy_cap_val;              /* è¾“å…¥æ•è·å€¼ */
lv_obj_t * ult_label;
void adc_scan_task(void *arg){
	uint16_t adcx, adcx2; float temp;
	while(1){
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
		if(current_task == 3){
			adcx = adc_get_result_average(ADC_ADCX_CHY, 10);
			pos ++;
			pos %= 320;
			line_points[pos].y = 240 - adcx / 18-120;
			line_v[0].x = pos;
			line_v[0].y = 0;
			line_v[1].x = pos;
			line_v[1].y = 240;
			lv_line_set_points(line2, line_v, 2);
			line_l[0].x = 0;
			line_l[0].y = 240 - adcx / 18 -120;//æ”¹
			line_l[1].x = 320;
			line_l[1].y = 240 - adcx / 18 -120;//æ”¹
			lv_line_set_points(line3, line_l, 2);
			lv_line_set_points(line1, line_points, 320);
			
			temp = (float)adcx * (3.3 / 4096);                  /* è·å–è®¡ç®—åçš„å¸¦å°æ•°çš„å®é™…ç”µå‹å€¼ï¼Œæ¯”å¦‚3.1111 */
			adcx = temp;
			temp -= adcx;                                       /* æŠŠå·²ç»æ˜¾ç¤ºçš„æ•´æ•°éƒ¨åˆ†å»æ‰ï¼Œç•™ä¸‹å°æ•°éƒ¨åˆ†ï¼Œæ¯”å¦‚3.1111-3=0.1111 */
			temp *= 1000; 
			adcx2 = temp;
			lv_label_set_text_fmt(adc_label, "ADC:%d.%dV", adcx, adcx2);
		}else if(current_task == 4){
			
			float distant;
			
			//g_timxchy_cap_sta = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			
			delay_us(20);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			
			vTaskDelay(1);
			uint32_t temp2 = 0, temp3 = 0;;

			if (g_timxchy_cap_sta & 0x80)           /* æˆåŠŸæ•è·åˆ°äº†ä¸€æ¬¡é«˜ç”µå¹³ */
			{
				temp2 = g_timxchy_cap_sta & 0x3F;
				temp2 *= 0xFFFF;                     /* æº¢å‡ºæ—¶é—´æ€»å’Œ */
				temp2 += g_timxchy_cap_val;          /* å¾—åˆ°æ€»çš„é«˜ç”µå¹³æ—¶é—´ */
				//lv_label_set_text_fmt(ult_label, "HIGH:%d us\r\n", temp2);     /* æ‰“å°æ€»çš„é«˜ç”µå¹³æ—¶é—´ */
				distant=(temp2*0.034)/2;  //å•ä½cm
				temp2 = distant;
				distant -= temp2;
				temp3 = distant * 1000;
				lv_label_set_text_fmt(ult_label, "distance:%d.%d\r\n", temp2, temp3);
				g_timxchy_cap_sta = 0;              /* å¼€å¯ä¸‹ä¸€æ¬¡æ•è· */
				
			}
		}
        xSemaphoreGive(xSemaphore);
		vTaskDelay(5);
	}
}

void lv_scr_sin()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    lv_obj_clean(lv_scr_act());
    current_task = 3;
    uint16_t adcx;

    for(int i = 0;i < 320 ;i++){
        line_points[i].x = i;
        line_points[i].y = 0;
    }
    adcx = adc_get_result_average(ADC_ADCX_CHY, 10);
    float temp = (float)adcx * (3.3 / 4096);
    lv_obj_t *adc_src = lv_obj_create(lv_scr_act());
    lv_obj_set_size(adc_src, 320, 240);
    lv_obj_align(adc_src, LV_ALIGN_TOP_LEFT,-10,-10);
    lv_obj_clear_flag(adc_src, LV_OBJ_FLAG_SCROLLABLE);
    
    adc_label = lv_label_create(adc_src);
    lv_obj_align(adc_label, LV_ALIGN_TOP_LEFT, 5, 0);
    lv_label_set_text_fmt(adc_label, "ADC:%d %f", adcx, temp);
    line1 = lv_line_create(adc_src);
    line2 = lv_line_create(adc_src);
    line3 = lv_line_create(adc_src);
    
    xSemaphoreGive(xSemaphore);
    
}
void lv_scr_ult()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    lv_obj_clean(lv_scr_act());
    current_task = 4;
    lv_obj_t *ult_src = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ult_src, 320, 240);
    ult_label = lv_label_create(ult_src);
    lv_obj_align(ult_label, LV_ALIGN_LEFT_MID, 5, 0);
    lv_label_set_text_fmt(ult_label, "distance:%d", 0);
    xSemaphoreGive(xSemaphore);
}

void change_scr()
{
    if(key_scan(0)== KEY0_PRES){
        current_task ++;
        if(current_task ==5){
            current_task=1;
        }
        
        
        switch (current_task){
            case 1:
                lv_scr_name();
                break;
            case 2:
                lv_scr_img();
                break;
            case 3:
                lv_scr_sin();
                break;
            case 4:
                lv_scr_ult();
                break;
            }
    }
   

}

