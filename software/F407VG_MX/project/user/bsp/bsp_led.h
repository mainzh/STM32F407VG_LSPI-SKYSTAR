#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "gpio.h"


/* 引脚定义 */
// 初始低电平为灭，高电平为亮

/* LED模式定义 */
#define LED(x)    do{ x ? \
                      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); \
                  }while(0)                                                          /* x = 1 , IO置高电平；x = 0 , IO置低电平 */

/* LED模式取反 */
#define LED_TOGGLE()    do{HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);}while(0)       /* LED = !LED */

/* 接口函数 */
void bsp_led_init(void);    /* LED初始化 */
void bsp_led_example(void);    /* LED试验 */

#endif    /* __BSP_LED_H */
