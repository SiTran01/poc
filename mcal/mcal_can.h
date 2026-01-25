#ifndef MCAL_CAN_H
#define MCAL_CAN_H

#include "stm32f407_base.h"

/* --- 1. C?U H?NH CAN MESSAGE --- */
typedef struct {
    uint32_t StdId;    // Standard Identifier (11-bit)
    uint8_t  DLC;      // Data Length Code (0-8 bytes)
    uint8_t  Data[8];  // M?ng ch?a d? li?u
} CAN_TxHeader_t;

typedef struct {
    uint32_t StdId;
    uint8_t  DLC;
    uint8_t  Data[8];
} CAN_RxHeader_t;

/* --- 2. API PROTOTYPES --- */

// Kh?i t?o CAN (Baudrate 500kbps v?i PCLK1 = 42MHz)
void MCAL_CAN_Init(CAN_TypeDef *pCANx);

// B?t/T?t CAN (R?i kh?i ch? d? Initialization)
void MCAL_CAN_Start(CAN_TypeDef *pCANx);

// C?u h?nh b? l?c (?? nh?n t?t c? tin nh?n ho?c l?c ID c? th?)
void MCAL_CAN_ConfigFilter(CAN_TypeDef *pCANx);

// G?i tin nh?n CAN (Blocking)
uint8_t MCAL_CAN_Transmit(CAN_TypeDef *pCANx, CAN_TxHeader_t *pHeader);

// Nh?n tin nh?n CAN t? FIFO 0
uint8_t MCAL_CAN_Receive(CAN_TypeDef *pCANx, CAN_RxHeader_t *pHeader);

#endif
