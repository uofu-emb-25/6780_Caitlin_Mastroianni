#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <hal_gpio.h>
#include <timers.h>

void ENABLE_RCC_TIM2(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Enable the timer 2 peripheral (TIM2) in the RCC.
}

void SET_TIM2_4Hz(void) {
    TIM2->PSC = 7999;  // Set prescaler to 7999 (Timer clock = 1 kHz)
    TIM2->ARR = 500;   // Set to 500 (4 Hz interrupt)
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt in NVIC
    TIM2->CR1 |= TIM_CR1_CEN;   // Enable TIM2 counter
}

void TIM2_IRQHandler(void){
    if (TIM2->SR & TIM_SR_UIF) {  // Check if update event occurred
        TIM2->SR &= ~TIM_SR_UIF;  // Clear update interrupt flag
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9); //Toggle between Green (PC8) and Orange (PC9) LEDs
    }
}