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
    GPIOB->MODER |= ((GPIOB -> MODER & ~(GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1) | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1));
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
    USART3->CR1 |= USART_CR1_RXNEIE;
    USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    USART3->CR1 |= USART_CR1_UE;
}

volatile char receivedChar;
volatile int charReceivedFlag = 0;

void USART3_4_IRQHandler(void) {
    receivedChar = USART3->RDR;  
    USART3_trans_Char(receivedChar);
    USART3_trans_Char('\r');
    USART3_trans_Char('\n'); 
    charReceivedFlag = 1;
}

void USART3_trans_Char(char inputChar){
    while(!(USART3->ISR & USART_ISR_TXE)){
    }
    USART3->TDR = inputChar;
}

void USART_trans_String(char inputString[]){
    for(int i = 0; i < strlen(inputString); i++) {
        USART3_trans_Char(inputString[i]);
    }
}


char USART3_read_Char(void) {
    while (!(USART3->ISR & USART_ISR_RXNE)); 
    return (char)(USART3->RDR); 
}

void USART3_input_LED(void) {
    static char ledChar = 0;
    static int recievedStatus = 0;

    if (charReceivedFlag) {
        charReceivedFlag = 0;

        if (!recievedStatus) {  
            switch (receivedChar) {
                case 'r': case 'R':
                case 'g': case 'G':
                case 'o': case 'O':
                case 'b': case 'B':
                    ledChar = receivedChar;
                    recievedStatus = 1;
                    USART_trans_String("\r\nEnter the number command (0 = off, 1 = on, 2 = toggle): ");
                    return;  
                default:
                    USART_trans_String("\r\nInvalid input. Use r, g, o, b.");
                    USART_trans_String("\r\nEnter a color letter (r, g, o, b): ");
                    return;
            }
        } else {  
            char numChar = receivedChar;  
            uint16_t ledPin = 0;

            switch (ledChar) {
                case 'r': 
                case 'R': 
                    ledPin = GPIO_PIN_6; 
                    break;
                case 'g': 
                case 'G': 
                    ledPin = GPIO_PIN_9; 
                    break;
                case 'o': 
                case 'O': 
                    ledPin = GPIO_PIN_8; 
                    break;
                case 'b': 
                case 'B': 
                    ledPin = GPIO_PIN_7; 
                    break;
            }

            switch (numChar) {
                case '0':
                    My_HAL_GPIO_WritePin(GPIOC, ledPin, GPIO_PIN_RESET);
                    USART_trans_String("\r\n LED is now off");
                    break;
                case '1':
                    My_HAL_GPIO_WritePin(GPIOC, ledPin, GPIO_PIN_SET);  // Turn ON LED
                    USART_trans_String("\r\nLED is now on");
                    break;
                case '2':
                    My_HAL_GPIO_WritePin(GPIOC, ledPin, GPIO_PIN_SET);
                    HAL_Delay(200);
                    My_HAL_GPIO_WritePin(GPIOC, ledPin, GPIO_PIN_RESET);
                    USART_trans_String("\r\nLED is toggled");
                    break;
                default:
                    USART_trans_String("\r\nInvalid command. Use 0, 1, or 2.");
                    USART_trans_String("\r\nEnter a color letter (r, g, o, b):");
                    ledChar = 0;  // Reset state
                    recievedStatus = 0;
                    return;
            }

            // Reset state after command execution
            ledChar = 0;
            recievedStatus = 0;
            USART_trans_String("\r\nEnter a color letter (r, g, o, b):");
        }
    }
}

void I2C2_Clock_Enable(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
}

void I2C2_Set_100Hz(void){
    I2C2->TIMINGR = (1U << 28) |  (0x13 << 0) | (0x0F << 8) | (0x02 << 16) | (0x04 << 20);
    I2C2->CR1 |= I2C_CR1_PE; 
}

#define WHO_AM_I      0x0F

void I2C2_Reading_Reg(void){
    I2C2->CR2 |= (1 << 13);
    I2C2->CR2 |= (0x69 << 1) | (1 << 16) | (0 << 10);

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
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
        } else {
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
            My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        }
    }

    I2C2->CR2 |= I2C_CR2_STOP;
}