#ifndef MCAL_TIMER_H
#define MCAL_TIMER_H

#include "stm32f407_base.h"

/* Channel Selection */
#define TIM_CHANNEL_1    1
#define TIM_CHANNEL_2    2
#define TIM_CHANNEL_3    3
#define TIM_CHANNEL_4    4

typedef struct{
	TIM_Type_Def *pTIMx;
	uint32_t Prescaler;
	uint32_t Period;
}TIM_Handle_t;


void MCAL_TIM_Base_Init(TIM_Handle_t *pTIMHandle);
void MCAL_TIM_PWM_Config(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t Pulse);


#endif
