#ifndef STM32F407_BASE
#define STM32F407_BASE

#include"stdint.h"

#define PERIPH_ADDR 								0x40000000UL
#define AHB1_ADDR										(PERIPH_ADDR + 0x20000UL)
#define GPIO_OFFSET									0x0400UL
#define RCC_ADDR										(AHB1_ADDR + 0x3800UL)

/* ===========================================
		==============  GPIO  ===================
	 =========================================== */
#define GPIOA_ADDR									(AHB1_ADDR + (GPIO_OFFSET*0))
#define GPIOB_ADDR									(AHB1_ADDR + (GPIO_OFFSET*1))
#define GPIOC_ADDR									(AHB1_ADDR + (GPIO_OFFSET*2))
#define GPIOD_ADDR									(AHB1_ADDR + (GPIO_OFFSET*3))
#define GPIOE_ADDR									(AHB1_ADDR + (GPIO_OFFSET*4))
#define GPIOF_ADDR									(AHB1_ADDR + (GPIO_OFFSET*5))
#define GPIOG_ADDR									(AHB1_ADDR + (GPIO_OFFSET*6))
#define GPIOH_ADDR									(AHB1_ADDR + (GPIO_OFFSET*7))
#define GPIOI_ADDR									(AHB1_ADDR + (GPIO_OFFSET*8))
#define GPIOJ_ADDR									(AHB1_ADDR + (GPIO_OFFSET*9))
#define GPIOK_ADDR									(AHB1_ADDR + (GPIO_OFFSET*10))

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_Type_Def;

//mapping
#define GPIOA ((GPIO_Type_Def *) GPIOA_ADDR)
#define GPIOB ((GPIO_Type_Def *) GPIOB_ADDR)
#define GPIOC ((GPIO_Type_Def *) GPIOC_ADDR)
#define GPIOD ((GPIO_Type_Def *) GPIOD_ADDR)
#define GPIOE ((GPIO_Type_Def *) GPIOE_ADDR)
#define GPIOF ((GPIO_Type_Def *) GPIOF_ADDR)
#define GPIOG ((GPIO_Type_Def *) GPIOG_ADDR)
#define GPIOH ((GPIO_Type_Def *) GPIOH_ADDR)
#define GPIOI ((GPIO_Type_Def *) GPIOI_ADDR)
#define GPIOJ ((GPIO_Type_Def *) GPIOJ_ADDR)
#define GPIOK ((GPIO_Type_Def *) GPIOK_ADDR)



/* ===========================================
											RCC
	============================================ */

typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t Reserved0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t Reserved1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t Reserved2;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t Reserved3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t Reserved4;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t Reserved5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t Reserved6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
}RCC_Type_Def;

// mapping
#define RCC ((RCC_Type_Def *) RCC_ADDR)



/*****************************
 * SysTick
 *****************************/
#define SCS_BASE            0xE000E000UL
#define SYSTICK_BASE        (SCS_BASE + 0x0010UL)

typedef struct {
	volatile uint32_t CTRL;
  volatile uint32_t LOAD;
  volatile uint32_t VAL;
  volatile uint32_t CALIB;
} SysTick_Type_Def;
//mapping
#define SysTick     ((SysTick_Type_Def *)SYSTICK_BASE)



/*****************************
 * Timer
 *****************************/
typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t Reserved0;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t Reserved1;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;
}TIMx_Type_Def;

#define APB1_ADDR 					PERIPH_ADDR
#define TIMx_OFFSET  				0x0400UL
#define TIM2_ADDR						(APB1_ADDR + (TIMx_OFFSET*0))
#define TIM3_ADDR						(APB1_ADDR + (TIMx_OFFSET*1))
#define TIM4_ADDR						(APB1_ADDR + (TIMx_OFFSET*2))
#define TIM5_ADDR						(APB1_ADDR + (TIMx_OFFSET*3))

//mapping
#define TIM2								((TIMx_Type_Def *) TIM2_ADDR)
#define TIM3								((TIMx_Type_Def *) TIM3_ADDR)
#define TIM4								((TIMx_Type_Def *) TIM4_ADDR)
#define TIM5								((TIMx_Type_Def *) TIM5_ADDR)



#endif
