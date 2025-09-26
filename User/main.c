/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-28
 * @brief       LVGL V8操作系统移植 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/ADC/gpio.h"
#include "./BSP/ADC/dma.h"
#include "./BSP/ula/ula.h"
#include "lvgl_demo.h"
#include "./BSP/TIM/tim.h"
#define  ADC1_DMA_Size	 1024      //ADC数据个数
 
uint32_t ADC1_ConvertedValue[ ADC1_DMA_Size ];   //定义ADC数据存储数组

#define  FFT_LENGTH		   1024
int SAM_FRE = 2000000; //采样率
int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    printf("1\n");
    led_init();                         /* ?????LED */
    key_init();                         /* ????????? */
    sram_init();                        /* SRAM初始化 */
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);   
    /* 初始化外部SRAM内存池 */
    printf("2\n");
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    printf("3\n");
    HAL_TIM_Base_Start(&htim3);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC1_ConvertedValue,ADC1_DMA_Size);
    printf("4\n");
    lvgl_demo();                        /* 运行FreeRTOS例程 */
    
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}