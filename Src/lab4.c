#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <assert.h>
#include <timers.h>


int lab4_main(void) {
    // System clock configuration
    SystemClock_Config();

    My_HAL_RCC_GPIOB_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef initStr = {GPIO_PIN_10 | GPIO_PIN_11,
        GPIO_MODE_AF_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOB, &initStr); 

    GPIO_InitTypeDef initStr_LED = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
   My_HAL_GPIO_Init(GPIOC, &initStr_LED);

    My_USART_HAL_GPIO_Init();

    USART3_Clock_Enable();

    USART3_Init();

    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 1);

   // USART3_trans_Char('B');

    // USART_trans_String("Enter a color letter (r, g, o, b): \r\n");

    USART_trans_String("\r\nEnter a color letter (r, g, o, b):");

   while(1){
    USART3_input_LED();
   }
}




