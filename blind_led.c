#include "blind_led.h"
#include "mcal_systick.h"

static void soft_delay(uint32_t cnt){
	for(volatile uint32_t i = 0; i< cnt; i++);
}

void ECU_LED_Init(LED_Handle_t *pLed){
	GPIO_Handle_t gpio_cfg;
	gpio_cfg.Port = pLed->pGPIOx;
	gpio_cfg.Pinconfig.GPIO_PinNumber = pLed->GPIO_PinNumber;
	
	gpio_cfg.Pinconfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpio_cfg.Pinconfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	gpio_cfg.Pinconfig.GPIO_OutputType = GPIO_OUTPUT_TYPE_PP;
	gpio_cfg.Pinconfig.GPIO_PUPD = GPIO_NO_PP;
	MCAL_GPIO_Init(&gpio_cfg);
}

void ECU_LED_SetState(LED_Handle_t *pLed, LED_State_t state){
	MCAL_GPIO_WritePin(pLed->pGPIOx, pLed->GPIO_PinNumber, state);
}

void ECU_LED_Toggle(LED_Handle_t *pLed){
	MCAL_GPIO_TogglePin(pLed->pGPIOx, pLed->GPIO_PinNumber);
}

void ECU_LED_Toggle_Delayms(LED_Handle_t *pLed, uint16_t ms){
	MCAL_GPIO_TogglePin(pLed->pGPIOx, pLed->GPIO_PinNumber);
	MCAL_Delay_ms(ms);
}


void ECU_LED_Breathing(LED_Handle_t *pLed, uint32_t speed_delay) {
    int duty_cycle;
    
    // 1. Sáng d?n (Duty cycle tang t? 0% lên 100%)
    for(duty_cycle = 0; duty_cycle < 100; duty_cycle++) {
        MCAL_GPIO_WritePin(pLed->pGPIOx, pLed->GPIO_PinNumber, GPIO_PIN_SET); // B?t
        soft_delay(duty_cycle * speed_delay); // Th?i gian b?t tang d?n
        
        MCAL_GPIO_WritePin(pLed->pGPIOx, pLed->GPIO_PinNumber, GPIO_PIN_RESET); // T?t
        soft_delay((100 - duty_cycle) * speed_delay); // Th?i gian t?t gi?m d?n
    }
    
    // 2. T?i d?n (Duty cycle gi?m t? 100% v? 0%)
    for(duty_cycle = 100; duty_cycle > 0; duty_cycle--) {
        MCAL_GPIO_WritePin(pLed->pGPIOx, pLed->GPIO_PinNumber, GPIO_PIN_SET);
        soft_delay(duty_cycle * speed_delay);
        
        MCAL_GPIO_WritePin(pLed->pGPIOx, pLed->GPIO_PinNumber, GPIO_PIN_RESET);
        soft_delay((100 - duty_cycle) * speed_delay);
    }
}
