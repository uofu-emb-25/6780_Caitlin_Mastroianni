#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <assert.h>
#include <timers.h>

int lab3_main(void) {
    // System clock configuration
    SystemClock_Config();

    // Enable peripheral clocks
    //My_HAL_RCC_GPIOC_CLK_ENABLE(); 
    //My_HAL_RCC_GPIOA_CLK_ENABLE();
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    //ENABLE_RCC_TIM2();
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //ENABLE_RCC_TIM3();
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //SET_TIM2_4Hz();
    TIM2->PSC = 7999;  // Set prescaler to 7999 (Timer clock = 1 kHz)
    TIM2->ARR = 1500;   // Set to 500 (4 Hz interrupt)
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    
    TIM2->CR1 |= TIM_CR1_CEN;   // Enable TIM2 counter

    
    //REDBLUEDIM();
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

    // Enable the timer counter
    TIM3->CR1 |= TIM_CR1_CEN;

    // Set to 20% of ARR value
    TIM3->CCR1 = 100;
    TIM3->CCR2 = 10;
    
    //GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7)) 
    //           | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; //Select alternate function mode on GPIOC pin 6 and 7    
    // Initialize PC6, PC7, PC8, PC9 as output pins (for LEDs)
    GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
   My_HAL_GPIO_Init(GPIOC, &initStr); 

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    
    GPIO_InitTypeDef redBlue = {GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &redBlue); 

    NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt in NVIC

    GPIOC->AFR[0] &= ~(0xF << GPIO_AFRL_AFRL7_Pos); //Select AF0 on PC7

    
   
     
    while(1){
        //HAL_Delay(400);
    }
}

void TIM2_IRQHandler(void){
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9); //Toggle between Green (PC8) and Orange (PC9) LEDs
    TIM2->SR &= ~TIM_SR_UIF;  // Clear update interrupt flag
}