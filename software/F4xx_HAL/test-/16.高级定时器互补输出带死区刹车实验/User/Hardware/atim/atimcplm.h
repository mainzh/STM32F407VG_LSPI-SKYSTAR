/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       高级定时器 驱动代码
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
 * V1.0 20211019
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __ATIMCPLM_H
#define __ATIMCPLM_H

#include "sys.h"

/***************************************** 高级定时器 互补输出带死区控制实验相关宏定义 *************************************************/

/* TIMX 互补输出定义 */

/* 主输出通道引脚 */
#define ATIM_TIMX_CPLM_CHY_GPIO_PORT            GPIOI
#define ATIM_TIMX_CPLM_CHY_GPIO_PIN             GPIO_PIN_5
#define ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0) /* PI口时钟使能 */

/* 互补输出通道引脚 */
#define ATIM_TIMX_CPLM_CHYN_GPIO_PORT           GPIOH
#define ATIM_TIMX_CPLM_CHYN_GPIO_PIN            GPIO_PIN_13
#define ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0) /* PH口时钟使能 */

/* 刹车输入引脚 */
#define ATIM_TIMX_CPLM_BKIN_GPIO_PORT           GPIOA
#define ATIM_TIMX_CPLM_BKIN_GPIO_PIN            GPIO_PIN_6
#define ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA口时钟使能 */

/* TIMX 引脚复用设置
 * 因为PI5、PH13、PA6默认并不是TIM8的功能脚, 必须开启复用, PI5、PH13、PA6才能用作TIM8的互补输出及刹车引脚。
 */
#define ATIM_TIMX_CPLM_CHY_GPIO_AF              GPIO_AF3_TIM8

#define ATIM_TIMX_CPLM                          TIM8
#define ATIM_TIMX_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX_CPLM_CHY_CCRY                 ATIM_TIMX_CPLM->CCR1
#define ATIM_TIMX_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)  /* TIM8 时钟使能 */

/******************************************************************************************/

void atim_timx_cplm_pwm_init(uint32_t Fcnt, uint32_t Count);                                   /* 高级定时器 互补输出 初始化函数 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg);                                     /* 高级定时器 互补输出 设置输出比较值 & 死区时间 */

#endif
