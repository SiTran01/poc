#ifndef PERI_INIT_H
#define PERI_INIT_H

#include "board_config.h"
#include "ecu_pedal.h"
#include "ecu_servo.h"
#include "ecu_oled.h"
#include "ecu_can.h"
#include "ecu_led.h"

//u know wat?
extern Pedal_Handle_t hPedal;
extern Servo_Handle_t hServo;
extern OLED_Handle_t hOled; 
extern CAN_Handle_t hCAN;
extern CAN_Handle_t hCAN2; // CAN 2 (Dashboard)
extern LED_Handle_t hGreenLed;

void PERI_Init(void);

#endif
