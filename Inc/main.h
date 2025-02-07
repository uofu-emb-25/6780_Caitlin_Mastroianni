#pragma once
#include <stdint.h>

void SystemClock_Config(void);
int lab1_main(void);
int lab2_main(void);
int lab3_main(void);
int lab4_main(void);
int lab5_main(void);
int lab6_main(void);
int lab7_main(void);


//Lab 1 Function
void My_HAL_RCC_GPIOC_CLK_ENABLE (void);
void My_HAL_RCC_GPIOA_CLK_ENABLE (void);
void My_HAL_RCC_SYSCFG_CLK_ENABLE (void);
void EXTI_rising_edge_trigger(void);
void SYSCFG_setup(void);