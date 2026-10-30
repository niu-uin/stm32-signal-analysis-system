#include "ad985x.h"
#include "stm32f4xx_hal.h" //改为自己32的hal库头文件
typedef unsigned int u16;
typedef unsigned char u8;
void AD9850_Init(void)
{
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_2 |GPIO_PIN_1 |GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    
            
	AD9850_WCLK_L;
	AD9850_FQUD_L;

	AD9850_RESET_L;
	AD9850_RESET_H;
	AD9850_Delay(0xffff);		
	AD9850_RESET_L;

	AD9850_WCLK_L;
	AD9850_WCLK_H;
	AD9850_Delay(0xffff);		
	AD9850_WCLK_L;

	AD9850_FQUD_L;
	AD9850_FQUD_H;
	AD9850_Delay(0xffff);		
	AD9850_FQUD_L;
}

void AD9850_Write(u8 w0,double frequence)
{
            
	unsigned char i;
	long int y;
	double x;
	u8 wdata;
	x=4294967295U/125;//适合125M晶振
	//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=frequence;
	//写w4数据
	wdata=(y>>=0);
	for(i=0;i<8;i++)
	{
            
		if(wdata&0x01)
			AD9850_DATA_H;
		else
			AD9850_DATA_L;
		AD9850_WCLK_H;
		wdata=wdata>>1;
		AD9850_WCLK_L;
	}
	wdata=(y>>8);
	for(i=0;i<8;i++)
	{
            
		if(wdata&0x01)
			AD9850_DATA_H;
		else
			AD9850_DATA_L;
		AD9850_WCLK_H;
		wdata=wdata>>1;
		AD9850_WCLK_L;
	}
	wdata=y>>16;
	for(i=0;i<8;i++)
	{
            
		if(wdata&0x01)
			AD9850_DATA_H;
		else
			AD9850_DATA_L;
		AD9850_WCLK_H;
		wdata=wdata>>1;
		AD9850_WCLK_L;
	}
	wdata=y>>24;
	for(i=0;i<8;i++)
	{
            
		if(wdata&0x01)
			AD9850_DATA_H;
		else
			AD9850_DATA_L;
		AD9850_WCLK_H;
		wdata=wdata>>1;
		AD9850_WCLK_L;
	}
	wdata=w0;
	for(i=0;i<8;i++)
	{
            
		if(wdata&0x01)
			AD9850_DATA_H;
		else
			AD9850_DATA_L;
		AD9850_WCLK_H;
		wdata=wdata>>1;
		AD9850_WCLK_L;
	}
	AD9850_FQUD_H;
//	AD9850_Delay(0xffff);		
	AD9850_FQUD_L;

}

void AD9850_Delay(unsigned int z) 
{
              
    
	for(;z>0;z--) 
    
	{
            ;} 
}
