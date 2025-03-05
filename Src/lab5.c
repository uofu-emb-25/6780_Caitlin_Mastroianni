#include <stdint.h>
#include <stm32f0xx_it.h>
#include <hal_gpio.h>
#include <timers.h>

#define WHO_AM_I      0x0F
#define CTRL_REG1     0x20
#define OUT_X_LOW     0x28
#define OUT_X_HIGH    0x29
#define OUT_Y_LOW     0x2A
#define OUT_Y_HIGH    0x2B
#define THRESHOLD     2000

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

    lab5_checkoff_one();

    Write_I2C(CTRL_REG1, 0x0F);

    while(1){
        int16_t x_axis = (Read_IC2(OUT_X_HIGH) << 8) | Read_IC2(OUT_X_LOW);

        int16_t y_axis = (Read_IC2(OUT_Y_HIGH) << 8) | Read_IC2(OUT_Y_LOW);

        My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

        if (x_axis > THRESHOLD) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); 
        } else if (x_axis < -THRESHOLD) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);  
        }

        if (y_axis > THRESHOLD) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        } else if (y_axis < -THRESHOLD) {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); 
        }
        HAL_Delay(100);
    }
}