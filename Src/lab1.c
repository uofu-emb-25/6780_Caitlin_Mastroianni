#include <stm32f0xx_hal.h>
#include <assert.h>
#include <hal_gpio.h>
#include <main.h>

int lab1_main(void) {
//HAL_Init(); // Reset of all peripherals, init the Flash and Systick
SystemClock_Config(); //Configure the system clock
/* This example uses HAL library calls to control
the GPIOC peripheral. You’ll be redoing this code
with hardware register access. */
My_HAL_RCC_GPIOC_CLK_ENABLE(); 
// Enable the GPIOC clock in the RCC
// Set up a configuration struct to pass to the initialization function
GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7,
GPIO_MODE_OUTPUT_PP,
GPIO_SPEED_FREQ_LOW,
GPIO_NOPULL};
My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9

GPIO_InitTypeDef initStr2 = {GPIO_PIN_0,
GPIO_MODE_INPUT,
GPIO_SPEED_FREQ_LOW,
GPIO_PULLDOWN};
My_HAL_GPIO_Init(GPIOA, &initStr2);

My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high
My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

while (1) {
    HAL_Delay(200); // Delay 200ms

    uint8_t buttonState = My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    if (!buttonState) {  // Active-low check
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    }
}
}