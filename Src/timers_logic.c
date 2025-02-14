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
// void TIM2_IRQHandler(void){
//     My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9); //Toggle between Green (PC8) and Orange (PC9) LEDs
//     TIM2->SR &= ~TIM_SR_UIF;  // Clear update interrupt flag
// }
void ENABLE_RCC_TIM3(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable TIM3 clock
}

void REDBLUEDIM(void){
    TIM3->PSC = 99;   // Prescaler for 80 kHz timer clock
    TIM3->ARR = 100;  // ARR for 800 Hz PWM period

     //Clearing and configuring channels as outputs
     TIM3->CCMR1 &= ~(3);
     TIM3->CCMR1 &= ~(3<<8);

    //Set ch1 to PWM output mode 2 (111)
     TIM3->CCMR1 |= (7 << 4);

     //Set ch2 to PWM output mode 1 (110)
     TIM3->CCMR1 |= (6 << 12);

    // Enable preload for channel 1 and channel 2
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

    // Set output enable bits for channel 1 and 2
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Set to 20% of ARR value
    TIM3->CCR1 = 20;
    TIM3->CCR2 = 20;
    
    GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7)) 
               | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; //Select alternate function mode on GPIOC pin 6 and 7

    
   // GPIOC->AFR[0] &= ~(0xF << GPIO_AFRL_AFRL7_Pos); //Select AF0 on PC7

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
