#include <stm32f0xx_it.h>
#include <assert.h>
#include <hal_gpio.h>
#include <main.h>

int iCount = 0;


void EXTI0_1_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) { // Confirm EXTI0 is the source
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9); // Toggle LEDs ONCE
        EXTI->PR |= (1 << 0); 
    }
}


int lab2_main(void) {
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

    // Initialize PA0 as input (for USER button)
    GPIO_InitTypeDef initStr2 = {
        GPIO_PIN_0,        
        GPIO_MODE_INPUT,    
        GPIO_SPEED_FREQ_LOW,
        GPIO_PULLDOWN       
    };
    My_HAL_GPIO_Init(GPIOA, &initStr2); 

    // Ensure EXTI0 configuration is correct
    assert((EXTI->RTSR & 0x1) == 0);   // Make sure rising-edge trigger is disabled initially
    EXTI_rising_edge_trigger();        // Enable rising-edge trigger for PA0
    assert((EXTI ->RTSR & 0x1) == 1);  // Check if rising-edge trigger is enabled

    // Enable EXTI0 interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_1_IRQn);  
    NVIC_SetPriority(EXTI0_1_IRQn, 0);   // Set priority for EXTI0 interrupt
    NVIC_SetPriority(SysTick_IRQn, 0);    // Set priority for SysTick interrupt

    // Setup SYSCFG to map PA0 to EXTI0
    SYSCFG_setup();
    assert((SYSCFG->EXTICR[0] & SYSCFG_EXTICR1_EXTI0) == 0);

    // Start with PC9 (Orange LED) ON
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

    // Main loop
    while(1) {
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        HAL_Delay(400);
    }
}