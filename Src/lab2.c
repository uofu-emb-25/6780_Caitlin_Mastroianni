#include <stm32f0xx_it.h>
#include <assert.h>
#include <hal_gpio.h>
#include <main.h>

int lab2_main(void) {
SystemClock_Config();

__HAL_RCC_GPIOC_CLK_ENABLE(); 
__HAL_RCC_GPIOA_CLK_ENABLE(); 

GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
GPIO_MODE_OUTPUT_PP,
GPIO_SPEED_FREQ_LOW,
GPIO_NOPULL};
My_HAL_GPIO_Init(GPIOC, &initStr); 

GPIO_InitTypeDef initStr2 = {
    GPIO_PIN_0,        
    GPIO_MODE_INPUT,    
    GPIO_SPEED_FREQ_LOW,
    GPIO_PULLDOWN       
};
My_HAL_GPIO_Init(GPIOA, &initStr2); 

assert((EXTI->RTSR & 0x1) == 0);
EXTI_rising_edge_trigger();
assert((EXTI ->RTSR & 0x1) == 1);

NVIC_EnableIRQ(EXTI0_1_IRQn);  
NVIC_SetPriority(EXTI0_1_IRQn, 0);

My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
while(1){
    HAL_Delay(600);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
}
}
