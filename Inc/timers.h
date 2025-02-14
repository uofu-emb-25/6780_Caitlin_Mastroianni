#pragma once
#include <stdint.h>

void ENABLE_RCC_TIM2(void);
void SET_TIM2_4Hz(void);
void TIM2_IRQHandler(void);
void ENABLE_RCC_TIM3(void);
void SET_TIM3_800Hz(void);
void SET_PMW_MODE(void);
void SET_TIM3_CHANNELS(void);
void SET_TIM3_DUTYCYCLE(void);
void SET_REDBLUE_PWM(void);
void START_TIM3(void);
void REDBLUEDIM(void);