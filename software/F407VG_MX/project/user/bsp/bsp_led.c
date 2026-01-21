#include "bsp.h"

/* LED初始化 */
void bsp_led_init(void)
{
    // 使用STM32CubeMX生产STM32配置代码时，在 gpio.c 中初始化所有IO
    //MX_GPIO_Init();
}

/* LED测试 */
void bsp_led_example(void)
{
      LED_TOGGLE();
      HAL_Delay(500);
}
