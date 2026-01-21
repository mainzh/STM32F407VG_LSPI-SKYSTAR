/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码    (TIM_PWMInput PWM输入)
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

#ifndef __GTIMPWMIN_H
#define __GTIMPWMIN_H

#include "sys.h"


 /* TIMX PWM输入定义 
  * 这里的PWM输入使用TIM5_CH1、TIM5_CH2
  */
#define GTIM_TIMX_PWMIN_CHY_GPIO_PORT         GPIOH
#define GTIM_TIMX_PWMIN_CHY_GPIO_PIN          GPIO_PIN_10
#define GTIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)       /* GPIOH时钟使能 */

/* TIMX 引脚复用设置
 * 因为PH10默认并不是TIM5的功能脚, 必须开启复用, PH10才能用作TIM5的PWM输入引脚
 */
#define GTIM_TIMX_PWMIN_CHY_GPIO_AF           GPIO_AF2_TIM5                                     /* 端口复用到TIM5 */

#define GTIM_TIMX_PWMIN                       TIM5                       
#define GTIM_TIMX_PWMIN_IRQn                  TIM5_IRQn
#define GTIM_TIMX_PWMIN_IRQHandler            TIM5_IRQHandler
#define GTIM_TIMX_PWMIN_CH1                   TIM_CHANNEL_1                                     /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_PWMIN_CH2                   TIM_CHANNEL_2                                     /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_PWMIN_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)        /* TIM5 时钟使能 */

/******************************************************************************************/
extern uint8_t  gtim_timx_pwmin_chy_sta ;           /* PWM输入状态，0：没有捕获，1：成功捕获 */
extern uint32_t gtim_timx_pwmin_chy_hval ;          /* PWM的高电平脉宽 */
extern uint32_t gtim_timx_pwmin_chy_cval ;          /* PWM的周期宽度 */

void gtim_timx_pwmin_chy_init(void);                                                            /* 通用定时器 PWM输入初始化函数 */

#endif
