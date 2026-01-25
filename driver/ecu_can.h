/* ecu_can.h */
#ifndef ECU_CAN_H
#define ECU_CAN_H

#include "mcal_can.h"
#include "mcal_gpio.h"
#include "stdio.h"

/* Struct qu?n lý ECU CAN */
typedef struct {
    CAN_TypeDef *pCANx;
} CAN_Handle_t;

/* API Init toàn di?n (C? GPIO + CAN) */
void ECU_CAN_Init(CAN_Handle_t *pHandle, CAN_TypeDef *CANx, 
                  GPIO_Type_Def *GPIO_Port, uint8_t TxPin, uint8_t RxPin, uint8_t AF_Mode);

/* Wrapper functions cho App dùng */
uint8_t ECU_CAN_Send(CAN_Handle_t *pHandle, uint32_t ID, uint8_t *pData, uint8_t Len);
uint8_t ECU_CAN_Receive(CAN_Handle_t *pHandle, uint32_t *pID, uint8_t *pData, uint8_t *pLen);

#endif
