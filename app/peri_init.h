#ifndef PERI_INIT_H
#define PERI_INIT_H

#include "board_config.h"
#include "ecu_pedal.h"
#include "ecu_servo.h"
#include "ecu_oled.h"

//u know wat?
extern Pedal_Handle_t hPedal;
extern Servo_Handle_t hServo;
extern OLED_Handle_t hOled; 

void PERI_Init(void);

#endif
