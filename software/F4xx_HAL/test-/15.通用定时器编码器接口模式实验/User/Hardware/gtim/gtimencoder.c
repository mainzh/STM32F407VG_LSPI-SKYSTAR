/**
 ****************************************************************************************************
 * @file        gtim.c
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
 ****************************************************************************************************
 */

#include "gtimencoder.h"


/************************************ 通用定时器 编码器接口 实验程序 **************************************/

TIM_HandleTypeDef g_timx_encode_chy_handle;         /* 定时器x句柄 */


/**
 * @brief       通用定时器 编码器接口初始化
 * @param       Fcnt: 定时器计数频率，范围 [Ft / 65536, Ft]，计算方法: Fcnt = Ft / (psc + 1)
 * @param       Count: 计数周期，范围 [1, 65536]，计算方法: Count = arr + 1
 * @retval      无
 * @note        定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft
 *              定时器中断时间 Tout = Count / Fcnt ,单位s
 */
void gtim_timx_encoder_chy_init(uint32_t Fcnt, uint32_t Count)
{
    uint16_t psc = 84000000 / Fcnt - 1;                         /* 预分频系数，范围 [0 , 65535] */
    uint16_t arr = Count - 1;                                   /* 自动重装载值，范围 [0, 65535] */
    
    GPIO_InitTypeDef gpio_init_struct;
    TIM_Encoder_InitTypeDef g_timx_encoder_chy_handle;
    
    GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE();                                    /* 开启通道1的IO时钟 */
    GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE();                                    /* 开启通道2的IO时钟 */
    GTIM_TIMX_ENCODER_CH1_CLK_ENABLE();                                         /* 开启通道1定时器时钟 */
    GTIM_TIMX_ENCODER_CH2_CLK_ENABLE();                                         /* 开启通道2定时器时钟 */
    
    gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH1_GPIO_PIN;                      /* 通道1的IO口，即编码器的A相 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                    /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                                        /* 不上下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                              /* 高速 */
    gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH1_GPIO_AF;                  /* IO复用，把IO作为定时器3功能 */
    HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH1_GPIO_PORT, &gpio_init_struct);  
    
    gpio_init_struct.Alternate = GTIM_TIMX_ENCODERCH2_GPIO_AF;
    gpio_init_struct.Pin = GTIM_TIMX_ENCODER_CH2_GPIO_PIN;                      /* 通道2的IO口，即编码器的B相 */
    HAL_GPIO_Init(GTIM_TIMX_ENCODER_CH2_GPIO_PORT, &gpio_init_struct);  
    
    g_timx_encode_chy_handle.Instance = GTIM_TIMX_ENCODER;                      /* 定时器3 */
    g_timx_encode_chy_handle.Init.Prescaler = psc;                              /* 定时器分频 */
    g_timx_encode_chy_handle.Init.Period = arr;                                 /* 自动重装载值 */
    g_timx_encode_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 不分频 */
    
    g_timx_encoder_chy_handle.EncoderMode = TIM_ENCODERMODE_TI12;               /* TI1,TI2都检测 */
    g_timx_encoder_chy_handle.IC1Polarity = TIM_ICPOLARITY_RISING;              /* 边沿检测器设置，非反向 */
    g_timx_encoder_chy_handle.IC1Selection = TIM_ICSELECTION_DIRECTTI;          /* 通道1映射到IC1*/
    g_timx_encoder_chy_handle.IC1Prescaler = TIM_ICPSC_DIV1;                    /* 不分频 */
    g_timx_encoder_chy_handle.IC1Filter = 10;                                   /* 滤波器设置 */
    
    g_timx_encoder_chy_handle.IC2Polarity = TIM_ICPOLARITY_RISING;              /* 边沿检测器设置，非反向 */
    g_timx_encoder_chy_handle.IC2Selection = TIM_ICSELECTION_DIRECTTI;          /* 通道2映射到IC2 */
    g_timx_encoder_chy_handle.IC2Prescaler = TIM_ICPSC_DIV1;                    /* 不分频 */
    g_timx_encoder_chy_handle.IC2Filter = 10;                                   /* 滤波器设置 */
    HAL_TIM_Encoder_Init(&g_timx_encode_chy_handle, &g_timx_encoder_chy_handle);
    
    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH1);     /* 开启编码器通道1 */
    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle,GTIM_TIMX_ENCODER_CH2);     /* 开启编码器通道2 */

    HAL_NVIC_SetPriority(GTIM_TIMX_ENCODER_INT_IRQn, 2, 0);                     /* 抢占优先级2，响应优先级0 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_ENCODER_INT_IRQn);                             /* 开启定时器3中断 */
    __HAL_TIM_CLEAR_FLAG(&g_timx_encode_chy_handle,TIM_IT_UPDATE);              /* 清除更新中断 */
    __HAL_TIM_ENABLE_IT(&g_timx_encode_chy_handle,TIM_IT_UPDATE);               /* 开启更新中断 */
    
}

/**
 * @brief       定时器3中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_ENCODER_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_encode_chy_handle);
}

volatile int g_timx_encode_count = 0;                               /* 用于记录溢出次数，向下溢出就-1，向上溢出就+1 */

/**
 * @brief       定时器中断回调函数
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle))   /* 判断CR1的DIR位 */
    {
        g_timx_encode_count--;                                      /* DIR位为1，也就是递减计数 */
    }
    else
    {
        g_timx_encode_count++;                                      /* DIR位为0，也就是递增计数 */
    }
}

/**
 * @brief       编码器计数
 * @param       无
 * @retval      当前总计数值
 */
int32_t gtim_get_encode(void)
{
    /* 计算当前总计数值，当前总计数值 = 计数器当前值 + 溢出次数*65536  */
    return ( int32_t )(__HAL_TIM_GET_COUNTER(&g_timx_encode_chy_handle) + g_timx_encode_count * 65536);
}
