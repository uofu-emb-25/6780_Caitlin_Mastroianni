#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <assert.h>
#include <timers.h>

int lab4_main(void) {
    // System clock configuration
    SystemClock_Config();

    My_HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef initStr = {GPIO_PIN_10 | GPIO_PIN_11,
        GPIO_MODE_AF_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOB, &initStr); 

    My_USART_HAL_GPIO_Init();

   USART3_Clock_Enable();

   USART3_Init();

   USART3_trans_Char('B');

   while(1){
   }
}