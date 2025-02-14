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

void ENABLE_RCC_TIM3(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable TIM3 clock
    (void)RCC->APB1ENR; // Small delay to allow clock to stabilize

    // Reset TIM3 (to make sure it's in a clean state)
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
}

void REDBLUEDIM(void){
    TIM3->PSC = 99;   // Prescaler for 80 kHz timer clock
    TIM3->ARR = 100;  // ARR for 800 Hz PWM period
    TIM3->CR1 = TIM_CR1_CEN;

    // Set channel 1 to PWM output mode 2 (110)
    TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);

    // Set channel 2 to PWM output mode 1 (101)
    TIM3->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);

    // Enable preload for channel 1 and channel 2
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

    // Set output enable bits for channel 1 and 2
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Set to 20% of ARR value
    TIM3->CCR1 = 20;
    TIM3->CCR2 = 20;

    // Set PC6 & PC7 to Alternate Function Mode
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2))); // Clear mode bits
    GPIOC->MODER |= (2 << (6 * 2)) | (2 << (7 * 2));    // Set to AF mode (10)

    // Set PC6 & PC7 to AF1 (TIM3_CH1 & TIM3_CH2)
    GPIOC->AFR[0] &= ~((0xF << (6 * 4)) | (0xF << (7 * 4))); // Clear AF bits
    GPIOC->AFR[0] |= (1 << (6 * 4)) | (1 << (7 * 4));        // Set AF1

    // Enable the timer counter
    TIM3->CR1 |= TIM_CR1_CEN;
}















// void SET_TIM3_800Hz(void){
//     TIM3->PSC = 99;   // Prescaler for 80 kHz timer clock
//     TIM3->ARR = 100;  // ARR for 800 Hz PWM period
//     TIM3->CR1 = TIM_CR1_CEN;
// }

// void SET_PMW_MODE(void){
//     // Set CC1S[1:0] and CC2S[1:0] to output mode
//     TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;
//     TIM3->CCMR1 &= ~TIM_CCMR1_CC2S;

//     // Set channel 1 to PWM output mode 2 (110)
//     TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);

//     // Set channel 2 to PWM output mode 1 (101)
//     TIM3->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);

//     // Enable preload for channel 1 and channel 2
//     TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
//     TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
// }

// void SET_TIM3_CHANNELS(void){
//     // Set output enable bits for channel 1 and 2
//     TIM3->CCER |= TIM_CCER_CC1E;
//     TIM3->CCER |= TIM_CCER_CC2E; 
// }


// void SET_TIM3_DUTYCYCLE(void){
//     // Set to 20% of ARR value
//     TIM3->CCR1 = 500;
//     TIM3->CCR2 = 20;
// }

// void SET_REDBLUE_PWM(void){
//     GPIOC->MODER &= ~((0x1 << 13) | (0x1 << 12) | (0x1 << 14) | (0x1 << 15)); // Clear PC6 & PC7
//     GPIOC->MODER |= ((0x1 << 15) | (0x1 << 13)); // Set PC6 & PC7 to Alternate function mode
//     GPIOC->AFR[0] &= ~((0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24)); // Set PC6 to AFO
//     GPIOC->AFR[0] &= ~((0x1 << 31) | (0x1 << 30) | (0x1 << 29) | (0x1 << 28)); // Set PC7 to AFO
// }

// void START_TIM3(void){
//     // Set prescaler (optional based on your frequency requirements)
//     TIM3->PSC = 0;  // Prescaler (set according to your clock speed)

//     // Set auto-reload value (ARR) (optional)
//     TIM3->ARR = 100;  // Set the ARR value (can be adjusted for desired PWM frequency)

//     // Enable the timer counter
//     TIM3->CR1 |= TIM_CR1_CEN;  // Enable TIM3 counter
// }
