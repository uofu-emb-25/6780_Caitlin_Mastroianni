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



void My_USART_HAL_GPIO_Init(void)
{
    GPIOB->MODER |= (GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1);
    GPIOB->AFR[1] |= ((4 << GPIO_AFRH_AFSEL10_Pos) | (4 << GPIO_AFRH_AFSEL11_Pos));
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

void My_HAL_RCC_GPIOB_CLK_ENABLE (void){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}

void USART3_Clock_Enable(void){
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
}

void USART3_Init(void){
    USART3->BRR = HAL_RCC_GetHCLKFreq()/115200;
    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 0);
    USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    USART3->CR1 |= USART_CR1_UE;
}

void USART3_trans_Char(char inputChar){
    while(!(USART3->ISR & (1 << 7))){
    }
    USART3->TDR = inputChar;
}

void USART_trans_String(char inputString[]){
    for(int i = 0; i < strlen(inputString); i++) {
        USART3_trans_Char(inputString[i]);
    }
}