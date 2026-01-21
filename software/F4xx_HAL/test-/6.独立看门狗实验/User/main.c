/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       独立看门狗 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台：正点原子 F407电机开发板
 * 在线视频：www.yuanzige.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 *
 ****************************************************************************************************
 */

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "iwdg.h"


int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    delay_ms(100);                          /* 延时100ms再初始化看门狗,LED0的变化"可见" */
    iwdg_init(IWDG_PRESCALER_64, 500);      /* 预分频数为64,重载值为500,溢出时间约为1s */
    LED0(0);                                /* 点亮LED0(红灯) */
    while(1)
    {
       if (key_scan(1) == KEY0_PRES)        /* 如果KEY0按下,则喂狗,支持连按 */
        {
            iwdg_feed();                    /* 喂狗 */
        }

        delay_ms(10);
    }
}
