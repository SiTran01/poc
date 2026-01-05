#include "mcal_timer.h"
#include "mcal_rcc.h"

void MCAL_TIM_Base_Init(TIM_Handle_t *pTIMHandle){
	
	TIMx_Type_Def *pTIMx = pTIMHandle->pTIMx;
	MCAL_RCC_TIM_ClockControl(pTIMx, 1);
	
	pTIMx->PSC = pTIMHandle->Prescaler;
	pTIMx->ARR = pTIMHandle->Period;
	pTIMx->CR1 &= ~(1U << 4); 

    // 4. [QUAN TR?NG] T?o s? ki?n Update gi? (Generate Update Event)
    // T?i sao? Vì ph?n c?ng STM32 ch? n?p giá tr? PSC t? d?m vào ho?t d?ng 
    // khi có s? ki?n Update. Ta ph?i kích ho?t bit UG (Update Generation) trong thanh ghi EGR.
  pTIMx->EGR |= (1U << 0);

  pTIMx->SR &= ~(1U << 0);
}

void MCAL_TIM_PWM_Config(TIM_Handle_t *pTIMHandle, uint8_t Channel, uint32_t Pulse)
{
    TIMx_Type_Def *pTIMx = pTIMHandle->pTIMx;
		MCAL_RCC_TIM_ClockControl(pTIMx, 1);
    switch(Channel)
    {
        case TIM_CHANNEL_1:
        {
            pTIMx->CCMR1 &= ~(7U << 4); 

            pTIMx->CCMR1 |= (6U << 4);

            pTIMx->CCMR1 |= (1U << 3);

            pTIMx->CCR1 = Pulse;
            pTIMx->CCER |= (1U << 0);
            
            break;
        }

        default:
            break;
    }
}
