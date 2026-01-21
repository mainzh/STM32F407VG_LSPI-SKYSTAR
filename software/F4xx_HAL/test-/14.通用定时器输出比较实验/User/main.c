/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-16
 * @brief       通用定时器输出比较 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "gtimoc.h"


int main(void)
{
    HAL_Init();                                     /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);             /* 设置时钟,168Mhz */
    delay_init(168);                                /* 延时初始化 */
    usart_init(115200);                             /* 串口初始化为115200 */
    led_init();                                     /* 初始化LED */
    gtim_timx_comp_pwm_init(1000000, 65536);   /* 1Mhz的计数频率 */
   
    while (1)
    {
        LED0_TOGGLE();                              /* LED0(红灯) 翻转 */
        delay_ms(10);
    }
}
