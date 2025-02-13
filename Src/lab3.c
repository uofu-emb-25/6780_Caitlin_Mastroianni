#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <assert.h>
#include <timers.h>


int lab3_main(void) {
    ENABLE_RCC_TIM2();
    SET_TIM2_4Hz();

    // System clock configuration
    SystemClock_Config();

    // Enable peripheral clocks
    My_HAL_RCC_GPIOC_CLK_ENABLE(); 
    My_HAL_RCC_GPIOA_CLK_ENABLE(); 
    My_HAL_RCC_SYSCFG_CLK_ENABLE();

    // Initialize PC6, PC7, PC8, PC9 as output pins (for LEDs)
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr); 

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

    while(1){
        HAL_Delay(400);
    }
}