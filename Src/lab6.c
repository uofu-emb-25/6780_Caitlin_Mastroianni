#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <timers.h>


int lab6_main(void) {
    SystemClock_Config();

    My_HAL_RCC_GPIOB_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

    GPIO_InitTypeDef initStr_LED = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr_LED);

    GPIO_InitTypeDef initStr_PA1 = {GPIO_PIN_1,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOA, &initStr_PA1);
    
    ADC_init();
    
    GPIO_InitTypeDef initStr_PA4 = {GPIO_PIN_4,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOA, &initStr_PA4);

    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    
    DAC->CR &= ~DAC_CR_TSEL1;  
    DAC->CR |= DAC_CR_EN1;

    const uint8_t sine_wave[32] = {254, 254, 254, 254, 254, 254, 254, 254, 
        254, 254, 254, 254, 254, 254, 254, 254,
        0,   0,   0,   0,   0,   0,   0,   0,  
        0,   0,   0,   0,   0,   0,   0,   0
    };

    uint8_t index = 0;

    while (1) {
        DAC->DHR8R1 = sine_wave[index];
        index = (index + 1) % 32;
        HAL_Delay(1);
    }
















    /*
    const uint8_t limitOne = 64;  
    const uint8_t limitTwo = 128;
    const uint8_t limitThree = 192;
    const uint8_t limitFour = 255;

    while(1){
        uint8_t adc_value = ADC1->DR;

        if (adc_value < limitOne) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        }
        else if (adc_value < limitTwo) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        }
        else if (adc_value < limitThree) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        }
        else if (adc_value < limitFour) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        }
        else {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
        }

        HAL_Delay(400); 
    }*/


}