#ifndef ECU_PEDAL_H
#define ECU_PEDAL_H

#include "stm32f407_base.h"
#include "mcal_adc.h" 
#include "stdio.h"
#include "mcal_gpio.h"


#define PEDAL_MIN_ADC       0    
#define PEDAL_MAX_ADC       4095  


typedef struct {
    ADC_Handle_t ADCHandle;
    uint8_t PedalPosition;
} Pedal_Handle_t;


void ECU_Pedal_Init(Pedal_Handle_t *pPedal, ADC_Type_Def *ADCx, uint8_t Channel, GPIO_Type_Def *Port, uint8_t PinNumber);


uint8_t ECU_Pedal_GetPosition(Pedal_Handle_t *pPedal);

#endif
