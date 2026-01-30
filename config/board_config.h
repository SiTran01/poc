#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f407_base.h"

// cfg 4 pedal
#define PEDAL_GPIO_PORT       GPIOA
#define PEDAL_GPIO_PIN        1         
#define PEDAL_ADC_INSTANCE    ADC1      
#define PEDAL_ADC_CHANNEL     1

//cfg 4 servo
#define SERVO_GPIO_PORT       GPIOD
#define SERVO_GPIO_PIN        12
#define SERVO_TIM_INSTANCE    TIM4
#define SERVO_TIM_CHANNEL     1
#define SERVO_GPIO_AF         2

//cfg 4 oled
#define OLED_GPIO_PORT        GPIOB
#define OLED_SCL_PIN          6
#define OLED_SDA_PIN          7
#define OLED_I2C_INSTANCE     I2C1
#define OLED_GPIO_AF          4

//cfg 4 can
#define CAN_GPIO_PORT         GPIOD
#define CAN_TX_PIN            1
#define CAN_RX_PIN            0
#define CAN_INSTANCE          CAN1
#define CAN_GPIO_AF           9

//cfg 4 can 2 (Dashboard - Receiver)
#define CAN2_GPIO_PORT        GPIOB
#define CAN2_TX_PIN           13
#define CAN2_RX_PIN           12
#define CAN2_INSTANCE         CAN2
#define CAN2_GPIO_AF          9

#endif
