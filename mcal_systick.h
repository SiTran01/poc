#ifndef MCAL_SYSTICK_H
#define MCAL_SYSTICK_H

#include "stm32f407_base.h"

void MCAL_SysTick_Init(uint32_t system_clock);
void MCAL_Delay_ms(uint32_t ms);
void SysTick_Handler(void);

#endif
