/**
 ****************************************************************************************************
 * @file        gtim.c
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
 ****************************************************************************************************
 */

#include "gtimpwmin.h"


uint8_t  gtim_timx_pwmin_chy_sta  = 0;  /* PWM输入状态 ，0：没有捕获，1：成功捕获*/
uint32_t gtim_timx_pwmin_chy_hval = 0;  /* PWM的高电平脉宽 */
uint32_t gtim_timx_pwmin_chy_cval = 0;  /* PWM的周期宽度 */

TIM_HandleTypeDef gtim_timx_pwmin_chy_handle;   /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX 通道Y PWM输入模式 初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @retval      无
 */
void gtim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct   = {0};
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};
    
    GTIM_TIMX_PWMIN_CHY_CLK_ENABLE();                                                           /* 使能TIM5时钟 */
    GTIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();                                                      /* 开启捕获IO的时钟 */

    gpio_init_struct.Pin = GTIM_TIMX_PWMIN_CHY_GPIO_PIN;                                        /* PWM输入IO */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                                    /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                                                      /* 下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                              /* 高速 */
    gpio_init_struct.Alternate = GTIM_TIMX_PWMIN_CHY_GPIO_AF;                                   /* 端口复用 */ 
    HAL_GPIO_Init(GTIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    gtim_timx_pwmin_chy_handle.Instance = GTIM_TIMX_PWMIN;                                         /* 定时器5 */
    gtim_timx_pwmin_chy_handle.Init.Prescaler = 84-1;                                              /* 定时器预分频系数 */
    gtim_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                              /* 递增计数模式 */
    gtim_timx_pwmin_chy_handle.Init.Period = 0xFFFFFFFF;                                           /* 自动重装载值 */
    HAL_TIM_IC_Init(&gtim_timx_pwmin_chy_handle);                                                  /* 初始化定时器 */
    
    /* 从模式配置，IT1触发更新 */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                                               /* 从模式：复位模式 */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                                                  /* 定时器输入触发源：TI1FP1 */
    slave_config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;                             /* 一开始捕获到上升沿复位计数值 */
    slave_config.TriggerFilter = 0;                                                             /* 不滤波 */
    HAL_TIM_SlaveConfigSynchro(&gtim_timx_pwmin_chy_handle, &slave_config);
    
    /* IC1捕获：上升沿触发TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;                              /* 上升沿捕获 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;                                    /* 选择输入端IC1映射到TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                                              /* 不分频 */
    tim_ic_pwmin_chy.ICFilter = 0;                                                              /* 不滤波 */
    HAL_TIM_IC_ConfigChannel(&gtim_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, GTIM_TIMX_PWMIN_CH1); /* 配置TIM5通道1 */

    /* IC2捕获：下降沿触发TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;                             /* 下降沿捕获 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;                                  /* 选择输入端IC2映射到TI1 */
    HAL_TIM_IC_ConfigChannel(&gtim_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, GTIM_TIMX_PWMIN_CH2); /* 配置TIM5通道2 */

    HAL_TIM_IC_Start_IT(&gtim_timx_pwmin_chy_handle, GTIM_TIMX_PWMIN_CH1);                         /* 开启TIM5的捕获通道1 */
    HAL_TIM_IC_Start_IT(&gtim_timx_pwmin_chy_handle, GTIM_TIMX_PWMIN_CH2);                         /* 开启TIM5的捕获通道2 */
    
    HAL_NVIC_SetPriority(GTIM_TIMX_PWMIN_IRQn, 2, 0);                                           /* 抢占优先级2，子优先级0 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_PWMIN_IRQn);                                                   /* 开启TIMx中断 */
}

/**
 * @brief       定时器更新中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_PWMIN_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&gtim_timx_pwmin_chy_handle);                                               /* 定时器共用处理函数 */
}

/**
 * @brief       定时器输入捕获中断处理回调函数
 * @param       htim:定时器句柄指针
 * @note        该函数在HAL_TIM_IRQHandler中会被调用，
                PWM输入属于输入捕获中的特例
 * @retval      无
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWMIN)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)                                           /* 通道1捕获到上升沿 */
        {
            __HAL_TIM_SET_COUNTER(&gtim_timx_pwmin_chy_handle,0);
            gtim_timx_pwmin_chy_cval = HAL_TIM_ReadCapturedValue(htim,GTIM_TIMX_PWMIN_CH1)+1;       /* 记录整个周期的计数值 */
            gtim_timx_pwmin_chy_sta = 1;
        }
        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)                                      /* 通道2捕获到下降沿 */
        {
            gtim_timx_pwmin_chy_hval = HAL_TIM_ReadCapturedValue(htim,GTIM_TIMX_PWMIN_CH2)+1;       /* 记录高电平时间的计数值 */
        }
    }
}
