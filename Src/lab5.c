#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <assert.h>
#include <timers.h>

#define WHO_AM_I      0x0F

int lab5_main(void) {
    // System clock configuration
    SystemClock_Config();

    My_HAL_RCC_GPIOB_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;


    GPIO_InitTypeDef initStr = {GPIO_PIN_11,
        GPIO_MODE_AF_OD,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOB, &initStr); 

    GPIO_InitTypeDef initStr2 = {GPIO_PIN_13,
        GPIO_MODE_AF_OD,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOB, &initStr2); 

    GPIOB->AFR[1] &= ~(0xF << 12 | 0xF << 20);
    GPIOB->AFR[1] |= (1 << 12) | (5 << 20);

    GPIO_InitTypeDef initStr3 = {GPIO_PIN_14,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr3); 
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);

    GPIO_InitTypeDef initStr4 = {GPIO_PIN_0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr4); 
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

    GPIO_InitTypeDef initStr_LED = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr_LED);

    I2C2->TIMINGR = (1U << 28) |  (0x13 << 0) | (0x0F << 8) | (0x02 << 16) | (0x04 << 20);
    I2C2->CR1 |= I2C_CR1_PE; 

    I2C2->CR2 &= ~(0x3FF);
    I2C2->CR2 &= ~(0xFF << 16);

    I2C2->CR2 |= (0x69 << 1) | (1 << 16);
    I2C2->CR2 &= ~(1 << 10);

    I2C2->CR2 |= (1 << 13);
    assert(I2C2->CR2 |= (1 << 13));

    
    while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
    if (I2C2->ISR & I2C_ISR_NACKF)
    {
        return;
    }
    

    I2C2->TXDR = WHO_AM_I;

    while (!(I2C2->ISR & I2C_ISR_TC));

    I2C2->CR2 |= (0x69 << 1) | (1 << 16) | (1 << 10) | (1 << 13);

    while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF)));

    if (I2C2->ISR & I2C_ISR_RXNE)
    {
        uint8_t data = I2C2->RXDR;

        if (data == 0xD3) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        } else {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
        }
    }

    I2C2->CR2 |= I2C_CR2_STOP;
}