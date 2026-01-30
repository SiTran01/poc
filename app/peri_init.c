#include "peri_init.h"

/* ÐÂY LÀ NOI BI?N ÐU?C T?O RA TRONG RAM */
Pedal_Handle_t hPedal;
Servo_Handle_t hServo;
OLED_Handle_t  hOled;
CAN_Handle_t hCAN;
CAN_Handle_t hCAN2;
LED_Handle_t hGreenLed;

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
									
		// 4. Init CAN
    ECU_CAN_Init(&hCAN, 
                 CAN_INSTANCE, 
                 CAN_GPIO_PORT, 
                 CAN_TX_PIN, 
                 CAN_RX_PIN, 
                 CAN_GPIO_AF);
								 
		// 5. Init CAN 2 (Dashboard)
    ECU_CAN_Init(&hCAN2, 
                 CAN2_INSTANCE, 
                 CAN2_GPIO_PORT, 
                 CAN2_TX_PIN, 
                 CAN2_RX_PIN, 
                 CAN2_GPIO_AF);
								 
		//6. Init greenLed
		ECU_LED_Init(&hGreenLed, GPIOD, 15);
}
