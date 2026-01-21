/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码    (TIM_OC 输出比较)
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

#ifndef __GTIMOC_H
#define __GTIMOC_H

#include "sys.h"


/* TIMX PWM输出定义 */

#define GTIM_TIMX_COMP_CHY_GPIO_PORT         GPIOD
#define GTIM_TIMX_COMP_CHY_GPIO_PIN          GPIO_PIN_12
#define GTIM_TIMX_COMP_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)      /* PD口时钟使能 */

/* TIMX 引脚复用设置
 * 因为PD12默认并不是TIM4的功能脚, 必须开启复用, PD12才能用作TIM4的PWM输出引脚
 */
#define GTIM_TIMX_COMP_CHY_GPIO_AF           GPIO_AF2_TIM4                                    /* 端口复用到TIM4 */

#define GTIM_TIMX_COMP                       TIM4                                             /* TIM4 */
#define GTIM_TIMX_COMP_IRQn                  TIM4_IRQn
#define GTIM_TIMX_COMP_IRQHandler            TIM4_IRQHandler
#define GTIM_TIMX_COMP_CHY                   TIM_CHANNEL_1                                    /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_COMP_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)       /* TIM4 时钟使能 */

/******************************************************************************************/

void gtim_timx_comp_pwm_init(uint32_t Fcnt, uint32_t Count);                                     /* 通用定时器 输出比较初始化函数 */

#endif
