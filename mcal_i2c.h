#ifndef MCAL_I2C_H
#define MCAL_I2C_H

#include "stm32f407_base.h"

/* --- 1. CÁC MACRO C?U HÌNH --- */
#define I2C_SCL_SPEED_SM     100000 
#define I2C_SCL_SPEED_FM     400000 

#define I2C_ACK_ENABLE       1
#define I2C_ACK_DISABLE      0

#define I2C_FM_DUTY_2        0       
#define I2C_FM_DUTY_16_9     1       

/* --- 2. STRUCT CONFIG --- */
typedef struct
{
    uint32_t I2C_SCLSpeed;
    uint8_t  I2C_DeviceAddress;
    uint8_t  I2C_AckControl;
    uint16_t I2C_FMDutyCycle;
} I2C_Config_t;

/* --- 3. HANDLE --- */
typedef struct
{
    I2C_Type_Def  *pI2Cx;
    I2C_Config_t I2C_Config;
} I2C_Handle_t;

/* --- 4. API --- */
void MCAL_I2C_Init(I2C_Handle_t *pI2CHandle);
void MCAL_I2C_PeripheralControl(I2C_Type_Def *pI2Cx, uint8_t EnOrDi);

/* [M?I] Hàm g?i d? li?u (Blocking Mode) */
void MCAL_I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr);

#endif