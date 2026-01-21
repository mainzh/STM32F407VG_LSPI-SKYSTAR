/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       按键输入 实验
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


int main(void)
{
    uint8_t key;

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */

    while(1)
    {
        key = key_scan(0);                  /* 得到键值 */

        if (key)
        {
            switch (key)
            {
                case KEY0_PRES:             /* 控制LED0(RED)翻转 */
                    LED0_TOGGLE();          /* LED0状态取反 */
                    break;

                case KEY1_PRES:             /* 控制LED1(GREEN)翻转 */
                    LED1_TOGGLE();          /* LED1状态取反 */
                    break;

                case KEY2_PRES:             /* 同时控制LED0, LED1翻转 */
                    LED0_TOGGLE();          /* LED0状态取反 */
                    LED1_TOGGLE();          /* LED1状态取反 */
                    break;
                default : break;
            }    
        }
        else
        {
            delay_ms(10);
        }
    }
}
