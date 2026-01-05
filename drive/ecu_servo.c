#include "ecu_servo.h"

/**
 * @brief  Kh?i t?o Servo
 * Hàm này s? t? d?ng Init GPIO và Init Timer PWM luôn.
 */
void ECU_Servo_Init(Servo_Handle_t *pServoHandle)
{
    // 1. C?u hình GPIO (Ðo?n này ông có th? tùy bi?n n?u d?i chân khác)
    // ? dây tôi dang gi? d?nh ông dùng TIM4 - CH1 -> PD12
    // N?u mu?n linh ho?t hon thì thêm tru?ng GPIO vào struct Handle
    GPIO_Handle_t ServoPin;
    ServoPin.Port = GPIOD;
    ServoPin.Pinconfig.GPIO_PinNumber = 12 ;
    ServoPin.Pinconfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    ServoPin.Pinconfig.GPIO_PinAltFunMode = 2; // AF2 cho TIM4
    ServoPin.Pinconfig.GPIO_OutputType = GPIO_OUTPUT_TYPE_PP;
    ServoPin.Pinconfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    ServoPin.Pinconfig.GPIO_PUPD = GPIO_NO_PP;
    
    MCAL_GPIO_Init(&ServoPin);

    // 2. C?u hình Timer Base (50Hz)
    // Luu ý: pTimerHandle ph?i du?c khai báo và gán pTIMx t? bên ngoài main
    pServoHandle->pTimerHandle->Prescaler = 83;     // 1MHz Clock
    pServoHandle->pTimerHandle->Period = 19999;     // 20ms Chu k?
    MCAL_TIM_Base_Init(pServoHandle->pTimerHandle);

    // 3. C?u hình PWM cho kênh tuong ?ng
    // Kh?i t?o ban d?u d? góc 0 d? (MIN_PULSE)
    MCAL_TIM_PWM_Config(pServoHandle->pTimerHandle, pServoHandle->PWM_Channel, SERVO_MIN_PULSE);

    // 4. Kích ho?t Timer ch?y ngay l?p t?c
    // Truy c?p th?ng vào thanh ghi CR1 thông qua pTIMx
    pServoHandle->pTimerHandle->pTIMx->CR1 |= (1 << 0);
}

/**
 * @brief  Quay Servo d?n góc ch? d?nh
 */
void ECU_Servo_WriteAngle(Servo_Handle_t *pServoHandle, uint8_t Angle)
{
    uint32_t ccr_val;

    // 1. Gi?i h?n góc an toàn
    if (Angle > SERVO_MAX_ANGLE) Angle = SERVO_MAX_ANGLE;

    // 2. Công th?c Map (Ánh x?) t? Ð? sang Ð? r?ng xung (us)
    // Pulse = Min + (Angle * (Max - Min) / 180)
    ccr_val = SERVO_MIN_PULSE + ((uint32_t)Angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / SERVO_MAX_ANGLE);

    // 3. Ghi vào thanh ghi CCR tuong ?ng
    // Chúng ta dùng hàm SetCompare ho?c truy c?p tr?c ti?p n?u ông chua vi?t hàm SetCompare
    // ? dây tôi vi?t ki?u switch-case d? h? tr? nhi?u kênh
    TIMx_Type_Def *pTIMx = pServoHandle->pTimerHandle->pTIMx;
    
    switch(pServoHandle->PWM_Channel)
    {
        case TIM_CHANNEL_1: pTIMx->CCR1 = ccr_val; break;
        case TIM_CHANNEL_2: pTIMx->CCR2 = ccr_val; break;
        case TIM_CHANNEL_3: pTIMx->CCR3 = ccr_val; break;
        case TIM_CHANNEL_4: pTIMx->CCR4 = ccr_val; break;
        default: break;
    }
}
