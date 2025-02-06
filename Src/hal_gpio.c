#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    for (uint8_t pin = 0; pin < 16; pin++)
    {
            GPIOx->MODER |= (GPIO_Init->Mode << (2 * pin));  

            if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD)
                GPIOx->OTYPER |= (1 << pin); 

            GPIOx->OSPEEDR |= (GPIO_Init->Speed << (2 * pin));

            GPIOx->PUPDR |= (GPIO_Init->Pull << (2 * pin));
    }
}


void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->MODER &= ~(0b11 * GPIO_Pin);
    GPIOx->OTYPER &= ~GPIO_Pin;
    GPIOx->OSPEEDR &= ~(0b11 * GPIO_Pin);
    GPIOx->PUPDR &= ~(0b11 * GPIO_Pin);
}



GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? 1 : 0;
}




void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET)
    {
        GPIOx->BSRR = GPIO_Pin; 
    }
    else 
    {
        GPIOx->BSRR = (GPIO_Pin << 16); 
    }
}



void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}

