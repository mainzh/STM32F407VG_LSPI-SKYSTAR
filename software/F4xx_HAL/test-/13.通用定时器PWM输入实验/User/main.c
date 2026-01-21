/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-16
 * @brief       通用定时器PWM输入 实验
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
#include "gtimpwmout.h"
#include "gtimpwmin.h"


int main(void)
{  
    uint8_t t = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    gtim_timx_pwm_chy_init(10000,5000);         /* 10k Hz计数频率，500ms的周期，其中高电平占一半也就是250ms = 250000us */
    gtim_timx_pwmin_chy_init();                 /* 初始化PWN输入，以1Mhz的频率计数，最大捕获周期 2^32 us */

    while (1)
    {
        t++;
        if(t % 20 ==0)
        {
            LED0_TOGGLE();
        }
        if(gtim_timx_pwmin_chy_sta == 1)            /* 成功捕获 */
        {
            gtim_timx_pwmin_chy_sta = 0;            /* 清除成功捕获标志 */
            
            printf("\r\n");
            printf("高电平时间：%d us\r\n",gtim_timx_pwmin_chy_hval);
            printf("PWM周期：%d us\r\n",gtim_timx_pwmin_chy_cval );
            printf("PWM频率：%d Hz\r\n",1000000/gtim_timx_pwmin_chy_cval);
        }
        delay_ms(10);
    }
}
