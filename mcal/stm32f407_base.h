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
}TIM_Type_Def;

#define APB1_ADDR 					PERIPH_ADDR
#define TIM_OFFSET  				0x0400UL
#define TIM2_ADDR						(APB1_ADDR + (TIM_OFFSET*0))
#define TIM3_ADDR						(APB1_ADDR + (TIM_OFFSET*1))
#define TIM4_ADDR						(APB1_ADDR + (TIM_OFFSET*2))
#define TIM5_ADDR						(APB1_ADDR + (TIM_OFFSET*3))

//mapping
#define TIM2								((TIM_Type_Def *) TIM2_ADDR)
#define TIM3								((TIM_Type_Def *) TIM3_ADDR)
#define TIM4								((TIM_Type_Def *) TIM4_ADDR)
#define TIM5								((TIM_Type_Def *) TIM5_ADDR)



/*****************************
 * ADC
 *****************************/
#define APB2_ADDR                   (PERIPH_ADDR + 0x00010000UL)

#define ADC1_ADDR                   (APB2_ADDR + 0x2000UL)
#define ADC2_ADDR                   (ADC1_ADDR + 0x100UL)
#define ADC3_ADDR                   (ADC1_ADDR + 0x200UL)

typedef struct {
	volatile uint32_t SR;     
  volatile uint32_t CR1;    
  volatile uint32_t CR2;    
  volatile uint32_t SMPR1;  
  volatile uint32_t SMPR2;  
  volatile uint32_t JOFR1;  
  volatile uint32_t JOFR2;  
  volatile uint32_t JOFR3;  
  volatile uint32_t JOFR4;  
  volatile uint32_t HTR;    
  volatile uint32_t LTR;    
  volatile uint32_t SQR1;   
  volatile uint32_t SQR2;   
  volatile uint32_t SQR3;   
  volatile uint32_t JSQR;   
  volatile uint32_t JDR1;   
  volatile uint32_t JDR2;   
  volatile uint32_t JDR3;   
  volatile uint32_t JDR4;   
  volatile uint32_t DR;     
} ADC_Type_Def;

// 3. Struct Common (Gi? nguyên)
typedef struct {
    volatile uint32_t CSR;      
    volatile uint32_t CCR;      
    volatile uint32_t CDR;      
} ADC_Common_Type_Def;

// 4. Mapping (Them ADC2, ADC3 vao)
#define ADC1         ((ADC_Type_Def *) ADC1_ADDR)
#define ADC2         ((ADC_Type_Def *) ADC2_ADDR)
#define ADC3         ((ADC_Type_Def *) ADC3_ADDR)

// ADC Common nam ngay sau ADC3 (Offset 0x300 so voi ADC1)
#define ADC_COMMON   ((ADC_Common_Type_Def *) (ADC1_ADDR + 0x300UL))



/*****************************
 * I2C
 *****************************/
#define I2C1_ADDR                   (APB1_ADDR + 0x5400UL)
#define I2C2_ADDR                   (I2C1_ADDR + 0x0400UL)
#define I2C3_ADDR                   (I2C1_ADDR + 0x0800UL)

typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;
}I2C_Type_Def;

//mapping
#define I2C1						((I2C_Type_Def *) I2C1_ADDR)
#define I2C2						((I2C_Type_Def *) I2C2_ADDR)
#define I2C3						((I2C_Type_Def *) I2C3_ADDR)

#endif
