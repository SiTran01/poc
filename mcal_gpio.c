#include "mcal_gpio.h"
#include "mcal_rcc.h"


void MCAL_GPIO_Init(GPIO_Handle_t *pHandle){
	uint8_t pin = pHandle->Pinconfig.GPIO_PinNumber;
	uint8_t pos = pin*2U;
	uint32_t tmp;
	if(pin > 15) return; // invalid pin
	MCAL_RCC_GPIO_ClockControl(pHandle->Port, 1);
	
	//Mode config
	tmp = pHandle->Port->MODER;
	tmp &= ~(0x3U << pos);			// clear 2 bit of pin
	tmp |= ((uint32_t)(pHandle->Pinconfig.GPIO_PinMode & 0x3U) << pos);   // write 2 - Here compiler will understand 
	pHandle->Port->MODER = tmp;
	
	/* Configure output type */
  if (pHandle->Pinconfig.GPIO_OutputType == GPIO_OUTPUT_TYPE_OD)
     pHandle->Port->OTYPER |= (1U << pin);
  else
     pHandle->Port->OTYPER &= ~(1U << pin);
		
	// speed config
	tmp = pHandle->Port->OSPEEDR;
	tmp &= ~(3U << pos);
	tmp |= ((uint32_t)(pHandle->Pinconfig.GPIO_PinSpeed & 0x3U) << pos);
	pHandle->Port->OSPEEDR = tmp;
	
	// PUPD config
	tmp = pHandle->Port->PUPDR;
	tmp &= ~(0x3U << pos);
	tmp |= ((uint32_t)(pHandle->Pinconfig.GPIO_PUPD & 0x3U) << pos);
	pHandle->Port->PUPDR = tmp;
	
	// afternate function config
	if (pHandle->Pinconfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
    uint32_t afr_index = pin / 8; 
    uint32_t afr_pos   = (pin % 8) * 4;

    tmp = pHandle->Port->AFR[afr_index];

    tmp &= ~(0xFU << afr_pos);
    tmp |= ((uint32_t)(pHandle->Pinconfig.GPIO_PinAltFunMode & 0xFU) << afr_pos);

    pHandle->Port->AFR[afr_index] = tmp;
	}
}


void MCAL_GPIO_WritePin(GPIO_Type_Def *Port, uint8_t PinNumber, uint8_t status){
	if(status == GPIO_PIN_SET)
		Port->BSRR = (1U << PinNumber);
	else
		Port->BSRR = (1U << (PinNumber + 16U));
}

void MCAL_GPIO_TogglePin(GPIO_Type_Def *Port, uint8_t PinNumber){
	Port->ODR ^= (1U << PinNumber);
}

uint8_t MCAL_GPIO_ReadPin(GPIO_Type_Def *Port, uint8_t PinNumber){
	uint32_t value = (Port->IDR >> PinNumber) & 0x1U;
	return (uint8_t)value;
}

