/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码    (TIM_InputCapture - 输入捕获)
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
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __GTIMIC_H
#define __GTIMIC_H

#include "sys.h"


/********************************* 通用定时器 输入捕获实验相关宏定义 *************************************/

 /* TIMX 输入捕获定义 
  * 这里的输入捕获使用定时器TIM5_CH1
  */
#define GTIM_TIMX_CAP_CHY_GPIO_PORT         GPIOH
#define GTIM_TIMX_CAP_CHY_GPIO_PIN          GPIO_PIN_10                                         /* 输入捕获引脚：PH10 */
#define GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)         /* GPIOH时钟使能 */

/* TIMX 引脚复用设置
 * 因为PH10默认并不是TIM5的功能脚, 必须开启复用, PH10才能用作TIM5的输入捕获引脚
 */
#define GTIM_TIMX_CAP_CHY_GPIO_AF           GPIO_AF2_TIM5                                       /* 端口复用到TIM5 */

#define GTIM_TIMX_CAP                       TIM5                                                /* 定时器5 */
#define GTIM_TIMX_CAP_IRQn                  TIM5_IRQn
#define GTIM_TIMX_CAP_IRQHandler            TIM5_IRQHandler
#define GTIM_TIMX_CAP_CHY                   TIM_CHANNEL_1                                       /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_CAP_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)          /* TIM5 时钟使能 */

/******************************************************************************************/
extern uint8_t  gtim_timx_cap_chy_sta;              /* 输入捕获状态 */
extern uint16_t gtim_timx_cap_chy_val;              /* 输入捕获值 */

void gtim_timx_cap_chy_init(uint32_t Fcnt, uint32_t Count);                                        /* 通用定时器 输入捕获初始化函数 */

#endif
