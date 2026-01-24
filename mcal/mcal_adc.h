#ifndef MCAL_ADC_H
#define MCAL_ADC_H

#include "stm32f407_base.h"

/* Struct c?u hình cho ADC */
typedef struct
{
    ADC_Type_Def *pADCx; // ADC1, ADC2...
    uint8_t Channel;     // Channel mu?n d?c (0-15)
} ADC_Handle_t;

/* Function Prototypes */
void MCAL_ADC_Init(ADC_Handle_t *pADCHandle);
uint16_t MCAL_ADC_Read(ADC_Handle_t *pADCHandle);

#endif