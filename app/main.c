#include "blind_led.h"
#include "system_manager.h"
#include "ecu_servo.h"
#include "mcal_systick.h"
#include "mcal_timer.h"

TIM_Handle_t hTim4;
Servo_Handle_t hThrottleServo; // Servo bu?m ga

int main()
{
	System_Init();
	//Led_Init();
	
	hTim4.pTIMx = TIM4;
	hThrottleServo.pTimerHandle = &hTim4;
	hThrottleServo.PWM_Channel = TIM_CHANNEL_1;
	
	ECU_Servo_Init(&hThrottleServo);
	
    while(1)
    {
        // Gi? servo v? 0 d?
        ECU_Servo_WriteAngle(&hThrottleServo, 0);
        MCAL_Delay_ms(1000);

        // Gi? servo v? 90 d?
        ECU_Servo_WriteAngle(&hThrottleServo, 90);
        MCAL_Delay_ms(1000);

        // Gi? servo v? 180 d?
        ECU_Servo_WriteAngle(&hThrottleServo, 180);
        MCAL_Delay_ms(1000);
    }
}
