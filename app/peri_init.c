#include "peri_init.h"

/* ÐÂY LÀ NOI BI?N ÐU?C T?O RA TRONG RAM */
Pedal_Handle_t hPedal;
Servo_Handle_t hServo;
OLED_Handle_t  hOled;

void PERI_Init(void) {
    // 1. Init Pedal
    ECU_Pedal_Init(&hPedal, 
                   PEDAL_ADC_INSTANCE, 
                   PEDAL_ADC_CHANNEL, 
                   PEDAL_GPIO_PORT, 
                   PEDAL_GPIO_PIN);

    // 2. Init Servo
    ECU_Servo_Init(&hServo, 
                   SERVO_TIM_INSTANCE, 
                   SERVO_TIM_CHANNEL, 
                   SERVO_GPIO_PORT, 
                   SERVO_GPIO_PIN, 
                   SERVO_GPIO_AF);

    // 3. Init OLED
    ECU_OLED_Init(&hOled, 
                  OLED_I2C_INSTANCE, 
                  OLED_GPIO_PORT, 
                  OLED_SCL_PIN, 
                  OLED_SDA_PIN, 
                  OLED_GPIO_AF);
}
