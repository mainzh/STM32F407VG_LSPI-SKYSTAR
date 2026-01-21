/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-15
 * @brief       基本定时器 驱动代码    (TIM_TimeBase 时基)
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

#include "led.h"
#include "btimint.h"


TIM_HandleTypeDef btim_timx_int_handler;         /* 定时器句柄 */

/**
  * @brief  基本定时器TIMX定时中断初始化函数
  * @param  Fcnt: 定时器计数频率，范围 [Ft / 65536, Ft]，计算方法: Fcnt = Ft / (psc + 1)
  * @param  Count: 计数周期，范围 [1, 65536]，计算方法: Count = arr + 1
  * @retval 无
  *
  * @note   基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
  *         基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟频率 Ft = 84Mhz
  *         定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft
  *         定时器中断时间 Tout = Count / Fcnt ,单位s
  */
void btim_timx_int_init(uint32_t Fcnt, uint32_t Count)
{
    uint16_t psc = 84000000 / Fcnt - 1;                         /* 预分频系数，范围 [0 , 65535] */
    uint16_t arr = Count - 1;                                   /* 自动重装载值，范围 [0, 65535] */
    
    btim_timx_int_handler.Instance = BTIM_TIMX_INT;                      /* 定时器X */
    btim_timx_int_handler.Init.Prescaler = psc;                          /* 预分频系数 */
    btim_timx_int_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 向上计数器 */
    btim_timx_int_handler.Init.Period = arr;                             /* 自动重装载值 */
    HAL_TIM_Base_Init(&btim_timx_int_handler);

    HAL_TIM_Base_Start_IT(&btim_timx_int_handler);                       /* 使能定时器x和定时器x更新中断 */
}

/**
 * @brief       定时器底册驱动，开启时钟，设置中断优先级
                此函数会被HAL_TIM_Base_Init()函数调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                             /* 使能TIM时钟 */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3);         /* 抢占优先级1，子优先级3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);                 /* 开启ITM3中断 */
    }
}

/**
 * @brief       定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&btim_timx_int_handler);                          /* 定时器中断公共处理函数 */
}

/**
 * @brief       定时器更新中断回调函数
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        LED1_TOGGLE();                                          /*LED1反转*/
    }
}
