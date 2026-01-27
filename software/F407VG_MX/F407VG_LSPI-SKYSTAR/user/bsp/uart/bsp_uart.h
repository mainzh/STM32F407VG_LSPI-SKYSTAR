#ifndef __BSP_SERIAL_H
#define __BSP_SERIAL_H

#include "usart.h"

#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <stdarg.h>


/* 接口定义 */
#define bsp_huartx          huart1
#define bsp_USARTx          USART1
#define bsp_hdma_usartx_rx  hdma_usart1_rx

#define USART_RX_LEN             256               /* 定义最大接收字节数 */

extern DMA_HandleTypeDef bsp_hdma_usartx_rx;

/* 接口函数 */
void bsp_uart_rx_DMA_start(void);    /* 串口接收DMA中断开始 */

void Vofa_FireWater(const char *format, ...);
void Vofa_JustFloat(float *_data, uint8_t _num);

#endif    /* __BSP_SERIAL_H */
