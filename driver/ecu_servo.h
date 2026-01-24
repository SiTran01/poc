#ifndef ECU_SERVO_H
#define ECU_SERVO_H

#include "stm32f407_base.h"
#include "mcal_timer.h"
#include "mcal_gpio.h"
#include "stdio.h"

#define SERVO_MIN_PULSE    500
#define SERVO_MAX_PULSE    2500
#define SERVO_MAX_ANGLE    180


typedef struct
{
    TIM_Handle_t TimerHandle;
    uint8_t       PWM_Channel;
} Servo_Handle_t;


void ECU_Servo_Init(Servo_Handle_t *pServoHandle, TIM_Type_Def *Timx, uint8_t Channel, GPIO_Type_Def *Port, uint8_t PinNumber, uint8_t AltFunc);

void ECU_Servo_WriteAngle(Servo_Handle_t *pServoHandle, uint8_t Angle);

#endif
