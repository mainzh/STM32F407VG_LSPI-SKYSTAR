#include "vofa.h"

// FireWater数据格式：用\n或\r\n 换行结尾，用逗号分隔通道
void Vofa_FireWater(const char *format, ...)
{
    static uint8_t txBuffer[100];
    uint32_t n;
    va_list args;
    va_start(args, format);
    n = vsnprintf((char *)txBuffer, 100, format, args);

    //....在此替换你的串口发送函数...........
    HAL_UART_Transmit_DMA(&vofa_huartx, (uint8_t *)txBuffer, n);
    //......................................

    va_end(args);
}

// JustFloat数据格式：数组地址和数据个数
void Vofa_JustFloat(float *_data, uint8_t _num)
{
    static uint8_t tempData[100];
    uint8_t temp_end[4] = {0x00, 0x00, 0x80, 0x7F};
    float temp_copy[_num];

    memcpy(&temp_copy, _data, sizeof(float) * _num);

    memcpy(tempData, (uint8_t *)&temp_copy, sizeof(temp_copy));
    memcpy(&tempData[_num * 4], &temp_end[0], 4);

    //....在此替换你的串口发送函数...........
    HAL_UART_Transmit_DMA(&vofa_huartx, tempData, (_num + 1) * 4);
    //......................................
}

/*...........示例..............
    //不能使用中文
    Vofa_FireWater("vofatx: 0.1, 10, 30, 120\n");
    printf("vofatx: 0.1, 10, 30, 120\n");
    
    float f1=0.5,f2=114.514;
    Vofa_FireWater("%f,%f\r\n", f1, f2);

    float f3[3]={88.77,0.66,55.44};
    Vofa_JustFloat(f3, 3);
*/
