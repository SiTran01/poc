#ifndef ECU_SERVO_H
#define ECU_SERVO_H

#include "stm32f407_base.h"
#include "mcal_timer.h"
#include "mcal_gpio.h"

/* C?u hình gi?i h?n d? r?ng xung (Ðon v?: us)
 * Servo chu?n: 0 d? = 1000us (1ms), 180 d? = 2000us (2ms)
 * M?t s? servo Tàu có th? là 500-2500, ông ch?nh ? dây n?u c?n.
 */
#define SERVO_MIN_PULSE    500
#define SERVO_MAX_PULSE    2500
#define SERVO_MAX_ANGLE    180

/* Struct qu?n lý Servo (Ð? sau này ông có g?n 4 con Servo cung d? qu?n lý) */
typedef struct
{
    TIM_Handle_t *pTimerHandle; // Timer nào qu?n lý servo này?
    uint8_t       PWM_Channel;  // Kênh PWM nào? (CHANNEL_1, 2...)
} Servo_Handle_t;

/* --- FUNCTION PROTOTYPES --- */

/* Hàm kh?i t?o toàn b? chân c?m và Timer cho Servo */
void ECU_Servo_Init(Servo_Handle_t *pServoHandle);

/* Hàm di?u khi?n góc quay (0 - 180) */
void ECU_Servo_WriteAngle(Servo_Handle_t *pServoHandle, uint8_t Angle);

#endif
