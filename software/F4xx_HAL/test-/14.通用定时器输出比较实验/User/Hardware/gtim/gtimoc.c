/**
 ****************************************************************************************************
 * @file        gtim.c
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
 ****************************************************************************************************
 */

#include "gtimoc.h"


TIM_HandleTypeDef gtim_timx_comp_pwm_handle;       /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX 输出比较模式 初始化函数（使用输出比较模式）
 * @param       Fcnt: 定时器计数频率，范围 [Ft / 65536, Ft]，计算方法: Fcnt = Ft / (psc + 1)
 * @param       Count: 计数周期，范围 [1, 65536]，计算方法: Count = arr + 1
 * @retval      无
 *
 * @note        基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟频率 Ft = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft
 *              定时器中断时间 Tout = Count / Fcnt ,单位s
 */
void gtim_timx_comp_pwm_init(uint32_t Fcnt, uint32_t Count)
{
    uint16_t psc = 84000000 / Fcnt - 1;                         /* 预分频系数，范围 [0 , 65535] */
    uint16_t arr = Count - 1;                                   /* 自动重装载值，范围 [0, 65535] */
    
    TIM_OC_InitTypeDef timx_oc_comp_pwm = {0};
    
    gtim_timx_comp_pwm_handle.Instance = GTIM_TIMX_COMP;                                           /* 定时器4 */
    gtim_timx_comp_pwm_handle.Init.Prescaler = psc;                                                /* 预分频系数 */
    gtim_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                               /* 递增计数模式 */
    gtim_timx_comp_pwm_handle.Init.Period = arr;                                                   /* 自动重装载值 */
    HAL_TIM_OC_Init(&gtim_timx_comp_pwm_handle);

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;                                                /* 比较输出模式翻转功能 */
    timx_oc_comp_pwm.Pulse = 0;                                                                 /* 默认比较值为0 */
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                                           /* 输出比较极性为低 */
    HAL_TIM_OC_ConfigChannel(&gtim_timx_comp_pwm_handle, &timx_oc_comp_pwm, GTIM_TIMX_COMP_CHY);   /* 初始化TIMx通道y */
    HAL_TIM_OC_Start_IT(&gtim_timx_comp_pwm_handle, GTIM_TIMX_COMP_CHY);                           /* 开启比较输出通道及其中断 */
    
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_OC_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct;
        
        GTIM_TIMX_COMP_CHY_GPIO_CLK_ENABLE();                                                   /* 开启IO时钟 */
        GTIM_TIMX_COMP_CHY_CLK_ENABLE();                                                        /* 开启TIM4时钟 */

        gpio_init_struct.Pin = GTIM_TIMX_COMP_CHY_GPIO_PIN;                                     /* PD12 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                                                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                          /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_COMP_CHY_GPIO_AF;                                /* IO复用为定时器功能 */
        HAL_GPIO_Init(GTIM_TIMX_COMP_CHY_GPIO_PORT, &gpio_init_struct);                         /* 初始化IO */
        
        HAL_NVIC_SetPriority(GTIM_TIMX_COMP_IRQn, 2, 0);                                        /* 抢占优先级2，子优先级0 */
        HAL_NVIC_EnableIRQ(GTIM_TIMX_COMP_IRQn);                                                /* 开启ITMx中断 */
    }
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_COMP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&gtim_timx_comp_pwm_handle);                                                /* 定时器共用处理函数 */
}

/**
 * @brief       定时器输出比较中断回调函数
 * @param       htim:定时器句柄
 * @note        该函数在发生输出翻转时会被调用
 * @retval      无
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    uint16_t tim_oc_val;
    
    if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
    {
        tim_oc_val = __HAL_TIM_GetCompare( htim,GTIM_TIMX_COMP_CHY ) + 1;                          /* 获取当前比较值 */
        
        /* 设置比较值，在上一次比较值的基础上递增200
         * 定时器采用1M Hz的计数频率，计数值递增200需要200us
         * 当计数值等于比较值的时候输出会发生翻转，因此，每200us会翻转一次
         * 输出波形的周期 = 2 * 200 = 400us，频率 = 1/周期 = 2.5k Hz
         * 需要改变频率的话只需要改变递增值即可
         */
        __HAL_TIM_SetCompare( htim ,GTIM_TIMX_COMP_CHY,( tim_oc_val + 200 ) % gtim_timx_comp_pwm_handle.Init.Period );
    }
}
