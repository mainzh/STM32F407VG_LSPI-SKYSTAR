/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码    (TIM_PWMOutput PWM输出)
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
 ****************************************************************************************************
 */

#include "gtimpwmout.h"


TIM_HandleTypeDef gtim_timx_pwm_chy_handle;     /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX 通道Y PWM输出 初始化函数（使用PWM模式1）
 * @param       Fcnt: 定时器计数频率，范围 [Ft / 65536, Ft]，计算方法: Fcnt = Ft / (psc + 1)
 * @param       Count: 计数周期，范围 [1, 65536]，计算方法: Count = arr + 1
 * @retval      无
 *
 * @note        基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟频率 Ft = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft
 *              定时器中断时间 Tout = Count / Fcnt ,单位s
 */
void gtim_timx_pwm_chy_init(uint32_t Fcnt, uint32_t Count)
{
    uint16_t psc = 84000000 / Fcnt - 1;                         /* 预分频系数，范围 [0 , 65535] */
    uint16_t arr = Count - 1;                                   /* 自动重装载值，范围 [0, 65535] */
    
    TIM_OC_InitTypeDef timx_oc_pwm_chy  = {0};                                              /* 定时器PWM输出配置 */
    
    gtim_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;                                         /* 定时器x */
    gtim_timx_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    gtim_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 向上计数模式 */
    gtim_timx_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    gtim_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                      /* 时钟分频因子 */
    HAL_TIM_PWM_Init(&gtim_timx_pwm_chy_handle);                                               /* 初始化PWM */

    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                               /* 模式选择PWM1 */
    timx_oc_pwm_chy.Pulse = ( arr + 1 ) / 2;                                                /* 设置比较值,此值用来确定占空比 */
                                                                                            /* 默认比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&gtim_timx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&gtim_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);                           /* 开启对应PWM通道 */
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        
        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();                                                /* 开启通道y的GPIO时钟 */
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();                                                     /* 开启定时器X时钟 */

        gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN;                                  /* 通道y的GPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                            /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                                                /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_PWM_CHY_GPIO_AF;                             /* IO复用为定时器功能 */
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);                      /* 初始化IO */
    }
}

/**
 * @brief       PWM设置CCR
 * @note
 *              CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
 *              占空比Duty = CCR / (ARR + 1)
 *              
 * @param       Compare 要写入的CCR的值，范围：0~100
 * @retval      无
 */
void pwm_set_compare(uint16_t Compare)
{
    /* 设置CCR的值 */
    __HAL_TIM_SET_COMPARE(&gtim_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, Compare);
}
