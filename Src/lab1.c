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
My_HAL_RCC_GPIOA_CLK_ENABLE(); 
// Enable the GPIOC clock in the RCC
// Set up a configuration struct to pass to the initialization function
GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7,
GPIO_MODE_OUTPUT_PP,
GPIO_SPEED_FREQ_LOW,
GPIO_NOPULL};
My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9

GPIO_InitTypeDef initStr2 = {
    GPIO_PIN_0,         // PA0
    GPIO_MODE_INPUT,    // Input mode
    GPIO_SPEED_FREQ_LOW,
    GPIO_PULLDOWN       // Pull-down to avoid floating
};
My_HAL_GPIO_Init(GPIOA, &initStr2);


My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high
// My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

uint32_t debouncer = 0; // Debouncer shift register

    while (1) {
        // Shift left to track past button states
        debouncer = (debouncer << 1);

        // Read the button state
        if (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
            debouncer |= 0x01; // Set lowest bit if button is high
        }

        // Detect a stable button press (transition from low to high)
        if (debouncer == 0xFFFFFFFF) {
        }

        // Detect when button is released (transition from high to low)
        if (debouncer == 0x00000000) {
        }
        if (debouncer == 0x7FFFFFFF) {
            My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
            My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        }   
    }
}