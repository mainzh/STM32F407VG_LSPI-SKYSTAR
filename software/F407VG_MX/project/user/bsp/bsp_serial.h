#ifndef __BSP_SERIAL_H
#define __BSP_SERIAL_H

#include "usart.h"
#include <stdio.h>


/* 接口定义 */
#define SERIAL_USARTx            USART1            /* 使用第几号串口 */
#define SERIAL_huartx            huart1            /* 所使用串口的句柄 */
#define SERIAL_hdma_usartx_rx    hdma_usart1_rx    /* DMA串口接收通道的句柄 */
#define SERIAL_USART_ISR         SR               /* USART的 状态寄存器SR */
#define SERIAL_USART_TDR         DR               /* USART的 发送数据寄存器TDR */

#define SERIAL_PRINTF_EN         1                 /*使能(1)/禁止(0) 串口printf*/
#define SERIAL_RX_EN             1                 /*使能(1)/禁止(0) 串口接收*/

#define USART_RX_LEN             256               /* 定义最大接收字节数 */

extern DMA_HandleTypeDef SERIAL_hdma_usartx_rx;

/* 接口函数 */
void bsp_serial_init(void);    /* 串口初始化 */

#endif    /* __BSP_SERIAL_H */
