#include "ecu_servo.h"


static uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ECU_Servo_Init(Servo_Handle_t *pServoHandle, TIM_Type_Def *Timx, uint8_t Channel, GPIO_Type_Def *Port, uint8_t PinNumber, uint8_t AltFunc){
	if (pServoHandle == NULL || Timx == NULL || Port == NULL) return;
	
	//GPIO Config
	GPIO_Handle_t gpio_cfg;
	gpio_cfg.Port = Port;
	gpio_cfg.Pinconfig.GPIO_PinNumber = PinNumber;
	gpio_cfg.Pinconfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	gpio_cfg.Pinconfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	gpio_cfg.Pinconfig.GPIO_PinAltFunMode = AltFunc;
	gpio_cfg.Pinconfig.GPIO_PUPD = GPIO_NO_PP;
  gpio_cfg.Pinconfig.GPIO_OutputType = GPIO_OUTPUT_TYPE_PP;
	
	MCAL_GPIO_Init(&gpio_cfg);
	
	//TIMER config
	pServoHandle->TimerHandle.pTIMx = Timx;
	pServoHandle->PWM_Channel = Channel;
	
	pServoHandle->TimerHandle.Prescaler = 84 - 1;
	pServoHandle->TimerHandle.Period = 20000 - 1;
//	pServoHandle->TimerHandle.CounterMode = TIM_COUNTER_UP;

	MCAL_TIM_Base_Init(&pServoHandle->TimerHandle);
	
	MCAL_TIM_PWM_Config(&pServoHandle->TimerHandle, Channel, 500);
}

/**
 * @brief  Quay Servo d?n góc ch? d?nh
 */
void ECU_Servo_WriteAngle(Servo_Handle_t *pServoHandle, uint8_t Angle)
{
    if (Angle > SERVO_MAX_ANGLE) Angle = SERVO_MAX_ANGLE;
    
    // Quy d?i góc sang xung (500 - 2500)
    uint32_t pulse = map(Angle, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
    
    // G?i l?i hàm Config d? c?p nh?t d? r?ng xung
    // Vì hàm Config cu c?a ông g?p c? Mode và CCR, nên ta dùng t?m nó d? ghi CCR
    MCAL_TIM_PWM_Config(&pServoHandle->TimerHandle, pServoHandle->PWM_Channel, pulse);
}
