#include <stm32f0xx_hal.h>
#include <assert.h>
#include <hal_gpio.h>
#include <main.h>

int lab2_main(void) {
SystemClock_Config();

My_HAL_RCC_GPIOC_CLK_ENABLE(); 
My_HAL_RCC_GPIOA_CLK_ENABLE(); 

GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
GPIO_MODE_OUTPUT_PP,
GPIO_SPEED_FREQ_LOW,
GPIO_NOPULL};
My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9

My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

while(1){
    HAL_Delay(600);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
}
}
