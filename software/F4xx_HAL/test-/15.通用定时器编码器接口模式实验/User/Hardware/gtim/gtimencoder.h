/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码    编码器接口模式(驱动接口1）
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

#ifndef __GTIMENCODER_H
#define __GTIMENCODER_H

#include "sys.h"

/************************************ 通用定时器 编码器接口实验相关宏定义 **************************************/

/* TIMX 编码器接口定义 */

#define GTIM_TIMX_ENCODER_CH1_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH1_GPIO_PIN          GPIO_PIN_6
#define GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0) /* PC口时钟使能 */

#define GTIM_TIMX_ENCODER_CH2_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH2_GPIO_PIN          GPIO_PIN_7
#define GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0) /* PC口时钟使能 */

/* TIMX 引脚复用设置
 * 因为PC6、PC7默认并不是TIM3的功能脚, 必须开启复用, 才能用作TIM3的编码器功能引脚
 */
#define GTIM_TIMX_ENCODERCH1_GPIO_AF            GPIO_AF2_TIM3                               /* 端口复用到TIM3 */
#define GTIM_TIMX_ENCODERCH2_GPIO_AF            GPIO_AF2_TIM3

#define GTIM_TIMX_ENCODER                       TIM3                                        /* TIM3 */
#define GTIM_TIMX_ENCODER_INT_IRQn              TIM3_IRQn
#define GTIM_TIMX_ENCODER_INT_IRQHandler        TIM3_IRQHandler
#define GTIM_TIMX_ENCODER_CH1                   TIM_CHANNEL_1                               /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH1_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

#define GTIM_TIMX_ENCODER_CH2                   TIM_CHANNEL_2                               /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH2_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

/*************************************************************************************/

void gtim_timx_encoder_chy_init(uint32_t Fcnt, uint32_t Count);                                /* 通用定时器 编码器接口初始化 */
int gtim_get_encode(void);                                                                  /* 获取编码器总计数值 */


#endif
