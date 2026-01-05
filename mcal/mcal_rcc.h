#ifndef MCAL_RCC_H
#define MCAL_RCC_H

#include "stm32f407_base.h"

/*****************************
 * GPIO clock enable macros
 *****************************/
#define GPIOA_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 0))
#define GPIOB_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 1))
#define GPIOC_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 2))
#define GPIOD_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 3))
#define GPIOE_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 4))
#define GPIOF_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 5))
#define GPIOG_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 6))
#define GPIOH_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 7))
#define GPIOI_CLOCK_ENABLE()  (RCC->AHB1ENR |= (1U << 8))

/*****************************
 * GPIO clock disable macros
 *****************************/
#define GPIOA_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 0))
#define GPIOB_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 1))
#define GPIOC_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 2))
#define GPIOD_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 3))
#define GPIOE_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 4))
#define GPIOF_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 5))
#define GPIOG_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 6))
#define GPIOH_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 7))
#define GPIOI_CLOCK_DISABLE()  (RCC->AHB1ENR &= ~(1U << 8))


/*****************************
 * TIM clock enaable macros
 *****************************/
#define TIM2_CLOCK_ENABLE()			(RCC->APB1ENR |= (1U << 0))
#define TIM3_CLOCK_ENABLE()			(RCC->APB1ENR |= (1U << 1))
#define TIM4_CLOCK_ENABLE()			(RCC->APB1ENR |= (1U << 2))
#define TIM5_CLOCK_ENABLE()			(RCC->APB1ENR |= (1U << 3))

// disable clock
#define TIM2_CLOCK_DISABLE()		(RCC->APB1ENR &= ~(1U << 0))
#define TIM3_CLOCK_DISABLE()		(RCC->APB1ENR &= ~(1U << 1))
#define TIM4_CLOCK_DISABLE()		(RCC->APB1ENR &= ~(1U << 2))
#define TIM5_CLOCK_DISABLE()		(RCC->APB1ENR &= ~(1U << 3))


// Hàm này d? kh?i t?o Clock h? th?ng
void MCAL_RCC_InitSystemClock(void);
void MCAL_RCC_GPIO_ClockControl(GPIO_Type_Def *Port, uint8_t EnOrDi);
void MCAL_RCC_TIM_ClockControl(TIMx_Type_Def *pTIMx, uint8_t EnOrDi);

#endif
