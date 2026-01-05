#include "mcal_rcc.h"

#define PLL_M 	8
#define PLL_N 	336
#define PLL_P 	2  // Main PLL (SYSCLK) = N/P = 336/2 = 168 MHz
#define PLL_Q 	7

// SYS Clock
void MCAL_RCC_InitSystemClock(void)
{
	// 1. B?t HSE (High Speed External - Th?ch anh ngoài 8MHz)
	RCC->CR |= (1 << 16); // Bit 16: HSEON
	
	// Ch? cho HSE ?n d?nh (Bit 17: HSERDY)
	while (!(RCC->CR & (1 << 17)));

	RCC->APB1ENR |= (1 << 28); // B?t Clock cho Power Interface (PWREN)

	*((volatile uint32_t *)0x40023C00) = (5 << 0) | (1 << 8) | (1 << 9) | (1 << 10); 
	
	RCC->CFGR |= (0U << 4);  // AHB Prescaler = 1 (Bits 7:4 = 0xxx)
	RCC->CFGR |= (5U << 10); // APB1 Prescaler = 4 (Bits 12:10 = 101)
	RCC->CFGR |= (4U << 13); // APB2 Prescaler = 2 (Bits 15:13 = 100)

	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | ((0) << 16) | (1 << 22) | (PLL_Q << 24);
	RCC->CR |= (1U << 24); // Bit 24: PLLON
	
	// Ch? PLL khóa (Bit 25: PLLRDY)
	while (!(RCC->CR & (1 << 25)));
	RCC->CFGR &= ~(3U << 0); // Clear 2 bit SW
	RCC->CFGR |= (2U << 0);  // Set SW = 10 (Ch?n PLL)

	while ((RCC->CFGR & (3U << 2)) != (2 << 2));
}

// GPIO clock
void MCAL_RCC_GPIO_ClockControl(GPIO_Type_Def *Port, uint8_t EnOrDi)
{
    if (EnOrDi)
    {
        if (Port == GPIOA) { GPIOA_CLOCK_ENABLE(); }
        else if (Port == GPIOB) { GPIOB_CLOCK_ENABLE(); }
        else if (Port == GPIOC) { GPIOC_CLOCK_ENABLE(); }
        else if (Port == GPIOD) { GPIOD_CLOCK_ENABLE(); }
        else if (Port == GPIOE) { GPIOE_CLOCK_ENABLE(); }
        else if (Port == GPIOF) { GPIOF_CLOCK_ENABLE(); }
        else if (Port == GPIOG) { GPIOG_CLOCK_ENABLE(); }
        else if (Port == GPIOH) { GPIOH_CLOCK_ENABLE(); }
        else if (Port == GPIOI) { GPIOI_CLOCK_ENABLE(); }
    }
    else
    {
        if (Port == GPIOA) { GPIOA_CLOCK_DISABLE(); }
        else if (Port == GPIOB) { GPIOB_CLOCK_DISABLE(); }
        else if (Port == GPIOC) { GPIOC_CLOCK_DISABLE(); }
        else if (Port == GPIOD) { GPIOD_CLOCK_DISABLE(); }
        else if (Port == GPIOE) { GPIOE_CLOCK_DISABLE(); }
        else if (Port == GPIOF) { GPIOF_CLOCK_DISABLE(); }
        else if (Port == GPIOG) { GPIOG_CLOCK_DISABLE(); }
        else if (Port == GPIOH) { GPIOH_CLOCK_DISABLE(); }
        else if (Port == GPIOI) { GPIOI_CLOCK_DISABLE(); }
    }
}

//TIMER clock
void MCAL_RCC_TIM_ClockControl(TIMx_Type_Def *pTIMx, uint8_t EnOrDi){
	if(EnOrDi)
	{
				if(pTIMx == TIM2){ TIM2_CLOCK_ENABLE(); }
				else if(pTIMx == TIM3) { TIM3_CLOCK_ENABLE(); }
				else if(pTIMx == TIM4) { TIM4_CLOCK_ENABLE(); }
				else if(pTIMx == TIM5) { TIM5_CLOCK_ENABLE(); }
	} else {
				if(pTIMx == TIM2){ TIM2_CLOCK_DISABLE(); }
				else if(pTIMx == TIM3) { TIM3_CLOCK_DISABLE(); }
				else if(pTIMx == TIM4) { TIM4_CLOCK_DISABLE(); }
				else if(pTIMx == TIM5) { TIM5_CLOCK_DISABLE(); }
	}
}

