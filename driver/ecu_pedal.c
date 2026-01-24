#include "ecu_pedal.h"


void ECU_Pedal_Init(Pedal_Handle_t *pPedal, ADC_Type_Def *ADCx, uint8_t Channel, GPIO_Type_Def *Port, uint8_t PinNumber){
	if(pPedal == NULL || ADCx == NULL | Port == NULL) return;
	
	// GPIO config
	GPIO_Handle_t gpio_cfg;
	gpio_cfg.Port = Port;
	gpio_cfg.Pinconfig.GPIO_PinNumber = PinNumber;
	gpio_cfg.Pinconfig.GPIO_PinMode = GPIO_MODE_ANALOG;
	gpio_cfg.Pinconfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	gpio_cfg.Pinconfig.GPIO_PUPD = GPIO_NO_PP;
	
	MCAL_GPIO_Init(&gpio_cfg);
	
	//ADC config
	pPedal->ADCHandle.pADCx = ADCx;
	pPedal->ADCHandle.Channel = Channel;
	
	MCAL_ADC_Init(&pPedal->ADCHandle);
	pPedal->PedalPosition = 0;
}

uint8_t ECU_Pedal_GetPosition(Pedal_Handle_t *pPedal)
{
    uint16_t raw_val = 0;
    uint32_t map_val = 0;

    
    raw_val = MCAL_ADC_Read(&pPedal->ADCHandle);

    
    if (raw_val < PEDAL_MIN_ADC) {
        raw_val = PEDAL_MIN_ADC;
    } 
    else if (raw_val > PEDAL_MAX_ADC) {
        raw_val = PEDAL_MAX_ADC;
    }

    // 3. Công th?c Map sang % (Linear Interpolation)
    // Công th?c: Output = (Input - InMin) * 100 / (InMax - InMin)
    // Ép ki?u uint32_t d? phép nhân *100 không b? tràn s?
    map_val = (uint32_t)(raw_val - PEDAL_MIN_ADC) * 100 / (PEDAL_MAX_ADC - PEDAL_MIN_ADC);

    pPedal->PedalPosition = (uint8_t)map_val;

    return pPedal->PedalPosition;
}
