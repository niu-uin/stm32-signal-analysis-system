#ifndef _AD9850_H
#define _AD9850_H
#include "stm32f4xx_hal.h"  //改成自己32的hal库头文件
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;

#define AD9850_CONTROL_PORT  GPIOC
#define AD9850_FQUD   GPIO_PIN_0  //改成自己连接的引脚
#define AD9850_WCLK   GPIO_PIN_1  //改成自己连接的引脚
#define AD9850_DATA   GPIO_PIN_3  //改成自己连接的引脚
#define AD9850_RST    GPIO_PIN_2  //改成自己连接的引脚

#define AD9850_WCLK_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_WCLK, GPIO_PIN_SET)
#define AD9850_WCLK_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_WCLK, GPIO_PIN_RESET)
#define AD9850_FQUD_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_FQUD, GPIO_PIN_SET)
#define AD9850_FQUD_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_FQUD, GPIO_PIN_RESET)
#define AD9850_DATA_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_DATA, GPIO_PIN_SET)
#define AD9850_DATA_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_DATA, GPIO_PIN_RESET)
#define AD9850_RESET_H	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_RST, GPIO_PIN_SET)
#define AD9850_RESET_L	HAL_GPIO_WritePin(AD9850_CONTROL_PORT, AD9850_RST, GPIO_PIN_RESET)


void AD9850_Delay(unsigned int z); 
void AD9850_Write(unsigned char w0,double frequence);
//void AD9850_Write(u8 w0,double frequence);
void AD9850_Init(void);

#endif
