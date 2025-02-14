#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t position = 0;
    uint32_t temp;

    while (((GPIO_Init->Pin) >> position) != 0)
  {
    if(((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) ||
    ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF))
        {
        temp = GPIOx->OSPEEDR;
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2));
        temp |= (GPIO_Init->Speed << (position * 2));
        GPIOx->OSPEEDR = temp;


        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;
        temp |= (((GPIO_Init->Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
        GPIOx->OTYPER = temp;
        }

    if((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
        {
        temp = GPIOx->AFR[position >> 3];
        temp &= ~(0xFu << ((position & 7) * 4));
        temp |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4));
        GPIOx->AFR[position >> 3] = temp;
    }

      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODER0 << (position * 2));
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2));
      GPIOx->MODER = temp;

      position++;
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

void EXTI_rising_edge_trigger(void){
    EXTI->IMR |= 0x0001;
    EXTI->RTSR |= 0x0001; 
}

void SYSCFG_setup(void){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
}
