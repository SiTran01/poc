#ifndef BLIND_LED_H
#define BLIND_LED_H

#include "mcal_gpio.h"
#include "stdint.h"

typedef enum{
	LED_ON 			= 1,
	LED_OFF 		= 0
}LED_State_t;

typedef struct{
	GPIO_Type_Def *pGPIOx;
	uint8_t GPIO_PinNumber;
}LED_Handle_t;

void ECU_LED_Init(LED_Handle_t *pLed);
void ECU_LED_SetState(LED_Handle_t *pLed, LED_State_t state);
void ECU_LED_Toggle(LED_Handle_t *pLed);
void ECU_LED_Toggle_Delayms(LED_Handle_t *pLed, uint16_t ms);
void ECU_LED_Breathing(LED_Handle_t *pLed, uint32_t speed_delay);

#endif
