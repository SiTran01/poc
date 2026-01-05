#include "mcal_systick.h"

static volatile uint32_t g_TickCount = 0;

void MCAL_SysTick_Init(uint32_t system_clock){
	SysTick->CTRL = 0;
	SysTick->LOAD = (system_clock / 1000U) - 1U;
	SysTick->VAL = 0;
	SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
}

void MCAL_Delay_ms(uint32_t ms)
{
    g_TickCount = ms;

    while (g_TickCount != 0);
}

void SysTick_Handler(void)
{
    if (g_TickCount > 0)
    {
        g_TickCount--; 
    }
}
