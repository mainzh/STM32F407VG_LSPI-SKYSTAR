/**
 ****************************************************************************************************
 * @file        gtim.c
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
 ****************************************************************************************************
 */

#include "gtimic.h"

TIM_HandleTypeDef gtim_timx_cap_chy_handle;      /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX 通道Y 输入捕获 初始化函数
 * @param       Fcnt: 定时器计数频率，范围 [Ft / 65536, Ft]，计算方法: Fcnt = Ft / (psc + 1)
 * @param       Count: 计数周期，范围 [1, 65536]，计算方法: Count = arr + 1
 * @retval      无
 *
 * @note        基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟频率 Ft = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft
 *              定时器中断时间 Tout = Count / Fcnt ,单位s
 */
void gtim_timx_cap_chy_init(uint32_t Fcnt, uint32_t Count)
{
    uint16_t psc = 84000000 / Fcnt - 1;                         /* 预分频系数，范围 [0 , 65535] */
    uint16_t arr = Count - 1;                                   /* 自动重装载值，范围 [0, 65535] */
    
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};

    gtim_timx_cap_chy_handle.Instance = GTIM_TIMX_CAP;                     /* 定时器5 */
    gtim_timx_cap_chy_handle.Init.Prescaler = psc;                         /* 定时器分频 */
    gtim_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    gtim_timx_cap_chy_handle.Init.Period = arr;                            /* 自动重装载值 */
    HAL_TIM_IC_Init(&gtim_timx_cap_chy_handle);

    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;                 /* 上升沿捕获 */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;             /* 映射到TI1上 */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;                       /* 配置输入分频，不分频 */
    timx_ic_cap_chy.ICFilter = 0;                                       /* 配置输入滤波器，不滤波 */
    HAL_TIM_IC_ConfigChannel(&gtim_timx_cap_chy_handle, &timx_ic_cap_chy, GTIM_TIMX_CAP_CHY);  /* 配置TIM5通道1 */

    __HAL_TIM_ENABLE_IT(&gtim_timx_cap_chy_handle, TIM_IT_UPDATE);         /* 使能更新中断 */
    HAL_TIM_IC_Start_IT(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);     /* 开始捕获TIM5的通道1 */
}

/**
 * @brief       通用定时器输入捕获初始化接口
                HAL库调用的接口，用于配置不同的输入捕获
 * @param       htim:定时器句柄
 * @note        此函数会被HAL_TIM_IC_Init()调用
 * @retval      无
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP)                                /* 输入通道捕获 */
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_CAP_CHY_CLK_ENABLE();                                 /* 使能TIMx时钟 */
        GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE();                            /* 开启捕获IO的时钟 */

        gpio_init_struct.Pin = GTIM_TIMX_CAP_CHY_GPIO_PIN;              /* 输入捕获的GPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLDOWN;                          /* 下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_CAP_CHY_GPIO_AF;         /* 端口复用 */ 
        HAL_GPIO_Init(GTIM_TIMX_CAP_CHY_GPIO_PORT, &gpio_init_struct);  /* 初始化IO */

        HAL_NVIC_SetPriority(GTIM_TIMX_CAP_IRQn, 2, 0);                 /* 抢占优先级2，子优先级0 */
        HAL_NVIC_EnableIRQ(GTIM_TIMX_CAP_IRQn);                         /* 开启ITMx中断 */
    }
}

/* 输入捕获状态(gtim_timx_cap_chy_sta)
 * [7]  :0,没有成功的捕获;1,成功捕获到一次.
 * [6]  :0,还没捕获到高电平;1,已经捕获到高电平了.
 * [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303
 *       注意:为了通用,我们默认ARR和CCRy都是16位寄存器,对于32位的定时器(如:TIM5),也只按16位使用
 *       按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒
 *
 *      (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒)
 */
uint8_t gtim_timx_cap_chy_sta = 0;                                          /* 输入捕获状态 */
uint16_t gtim_timx_cap_chy_val = 0;                                         /* 输入捕获值 */

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_CAP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&gtim_timx_cap_chy_handle);                         /* 定时器HAL库共用处理函数 */
}

/**
 * @brief       定时器输入捕获中断处理回调函数
 * @param       htim:定时器句柄指针
 * @note        该函数在HAL_TIM_IRQHandler中会被调用
 * @retval      无
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP)
    {
        if ((gtim_timx_cap_chy_sta & 0X80) == 0)                            /* 还未成功捕获 */
        {
            if (gtim_timx_cap_chy_sta & 0X40)                               /* 捕获到一个下降沿 */
            {
                gtim_timx_cap_chy_sta |= 0X80;                              /* 标记成功捕获到一次高电平脉宽 */
                gtim_timx_cap_chy_val = HAL_TIM_ReadCapturedValue(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);  /* 获取当前的捕获值 */
                TIM_RESET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                      /* 一定要先清除原来的设置 */
                TIM_SET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_RISING); /* 配置TIM5通道1上升沿捕获 */
            }
            else                                                        /* 还未开始,第一次捕获上升沿 */
            {
                gtim_timx_cap_chy_sta = 0;                                  /* 清空 */
                gtim_timx_cap_chy_val = 0;
                gtim_timx_cap_chy_sta |= 0X40;                              /* 标记捕获到了上升沿 */
                __HAL_TIM_DISABLE(&gtim_timx_cap_chy_handle);              /* 关闭定时器5 */
                __HAL_TIM_SET_COUNTER(&gtim_timx_cap_chy_handle, 0);       /* 定时器5计数器清零 */
                TIM_RESET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                       /* 一定要先清除原来的设置 */
                TIM_SET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_FALLING); /* 定时器5通道1设置为下降沿捕获 */
                __HAL_TIM_ENABLE(&gtim_timx_cap_chy_handle);               /* 使能定时器5 */
            }
        }
    }
}

/**
 * @brief       定时器更新中断回调函数
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP)
    {
        if ((gtim_timx_cap_chy_sta & 0X80) == 0)                            /* 还未成功捕获 */
        {
            if (gtim_timx_cap_chy_sta & 0X40)                               /* 已经捕获到高电平了 */
            {
                if ((gtim_timx_cap_chy_sta & 0X3F) == 0X3F)                 /* 高电平太长了 */
                {
                    TIM_RESET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                     /* 一定要先清除原来的设置 */
                    TIM_SET_CAPTUREPOLARITY(&gtim_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_RISING);/* 配置TIM5通道1上升沿捕获 */
                    gtim_timx_cap_chy_sta |= 0X80;                          /* 标记成功捕获了一次 */
                    gtim_timx_cap_chy_val = 0XFFFF;
                }
                else                                                    /* 累计定时器溢出次数 */
                {
                    gtim_timx_cap_chy_sta++;
                }
            }
        }
    }
}
