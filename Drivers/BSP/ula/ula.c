#include "ula.h"
#include "./BSP/TIMER/gtim.h"
#include "./SYSTEM/sys/sys.h"
void ula_init(){
    GPIO_InitTypeDef gpio_init_struct;  
    gpio_init_struct.Pin = GPIO_PIN_3;                       /* WKUPӽޅ */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                    /* ˤɫ */
    gpio_init_struct.Pull = GPIO_PULLUP;                      /* Ђ- */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ٟ̙ */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct); 
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    gtim_timx_cap_chy_init(0xFFFF, 84 - 1);
}
