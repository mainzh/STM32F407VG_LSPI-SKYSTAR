#include "bsp.h"

/* Use "printf" function without definition "use MicroLIB" ------------------------------------------------------------------*/

/* 使用AC6编译器
 */
#if (__ARMCC_VERSION >= 6010050)
__asm(".global __use_no_semihosting\n\t");    /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* 声明main函数为无参数格式，否则部分例程可能出现半主机模式 */
#else
/* 使用AC5编译器时
 */
#pragma import(__use_no_semihosting)    /* 声明不使用半主机模式 */

/* 定义__FILE
 */
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

/* 不使用半主机模式，至少需重定义 __ttywrch函数、_sys_exit函数、_sys_command_string函数，以同时兼容AC5和AC6模式 */

#endif

/*重定义 __ttywrch函数
 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 重定义 _sys_exit函数
 * 避免使用半主机模式
 */
void _sys_exit(int x)
{
    x = x;
}

/*重定义 _sys_command_string函数
 */
char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* __FILE在stdio.h中定义
 */
FILE __stdout;

/* printf函数重定向
 * 重定义fputc函数
 * 供printf函数最终调用，输出字符串 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&bsp_huartx, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    
    return ch;
}

/* Use "printf" function without definition "use MicroLIB" ------------------------------------------------------------------*/

/* USART_RX ------------------------------------------------------------------*/

/* 接收缓冲区, 最大USART_REC_LEN个字节，用于存储完整的数据帧 */
uint8_t g_usart_rx_buf[USART_RX_LEN];

/* 串口初始化 */
//MX_USART1_UART_Init();

/* 串口接收DMA中断开始 */
void bsp_uart_rx_DMA_start(void)
{
    /* 开启串口接收空闲DMA中断 */
    HAL_UARTEx_ReceiveToIdle_DMA(&bsp_huartx, (uint8_t *)g_usart_rx_buf, sizeof(g_usart_rx_buf));
    /* 关闭DMA传输过半中断(否则也将触发RxEventCallback回调函数) */
    __HAL_DMA_DISABLE_IT(&bsp_hdma_usartx_rx, DMA_IT_HT);    /* (DMA通道的指针地址 , 要关闭的中断) */
}

/* UART接收事件回调函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* 判断使用此回调函数的串口 */
    if(huart->Instance == bsp_USARTx)
    {
        /* 发送接收到的数据 */
        HAL_UART_Transmit_DMA(&bsp_huartx, (uint8_t *)g_usart_rx_buf, Size);
        
        /* 数据的处理逻辑 */
        
        
        /* 再次开启串口接收DMA中断 */
        bsp_uart_rx_DMA_start();
    }
}

/* USART_RX ------------------------------------------------------------------*/
