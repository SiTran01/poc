#include "mcal_gpio.h"
#include "blind_led.h"
#include "mcal_systick.h"

void Led_Init()
{
    GPIO_Handle_t led12;
    led12.Port = GPIOD;
    led12.Pinconfig.GPIO_PinNumber = 12;
    led12.Pinconfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
    led12.Pinconfig.GPIO_OutputType = GPIO_OUTPUT_TYPE_PP;
    led12.Pinconfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
		
		//config for PWM
//		led12.Port = GPIOD;
//		led12.Pinconfig.GPIO_PinNumber = 12;
//		led12.Pinconfig.GPIO_PinMode = GPIO_MODE_ALTFN; // <--- QUAN TR?NG
//		led12.Pinconfig.GPIO_PinAltFunMode = 2; // AF2 là dành cho TIM3, TIM4, TIM5
	
    MCAL_GPIO_Init(&led12);
}


void blind_led_1s(){
	MCAL_GPIO_WritePin(GPIOD, 12, GPIO_PIN_SET);
	MCAL_Delay_ms(1000);
	MCAL_GPIO_WritePin(GPIOD, 12, GPIO_PIN_RESET);
	MCAL_Delay_ms(1000);
}
