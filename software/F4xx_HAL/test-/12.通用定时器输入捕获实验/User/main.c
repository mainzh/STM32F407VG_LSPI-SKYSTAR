/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-16
 * @brief       通用定时器输入捕获 实验
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
#include "gtimic.h"


int main(void)
{  
    uint32_t temp = 0;
    uint8_t t = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    gtim_timx_pwm_chy_init(10000,5000);         /* 10k HZ计数频率，500ms的周期，占空比为50%，也就是高电平时间 = 250ms = 250000us */
    gtim_timx_cap_chy_init(1000000, 65536);     /* 以1Mhz的频率计数 捕获 */

    while (1)
    {
        if (gtim_timx_cap_chy_sta & 0X80)           /* 成功捕获到了一次高电平 */
        {
            temp = gtim_timx_cap_chy_sta & 0X3F;
            temp *= 0xFFFF;                     /* 溢出时间总和 */
            temp += gtim_timx_cap_chy_val;          /* 得到总的高电平时间 */
            printf("HIGH:%d us\r\n", temp);     /* 打印总的高点平时间 */
            gtim_timx_cap_chy_sta = 0;              /* 开启下一次捕获 */
        }

        t++;
        if (t > 20)                             /* 200ms进入一次 */
        {
            t = 0;
            LED0_TOGGLE();                      /* LED0闪烁 ,提示程序运行 */
        }
        delay_ms(10);
    }
}
