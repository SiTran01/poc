#include "mcal_i2c.h"
#include "mcal_rcc.h" 

/* 1. Hàm b?t/t?t Clock ngo?i vi I2C (Quan tr?ng, main g?i cái này b? l?i) */
void MCAL_I2C_PeripheralControl(I2C_Type_Def *pI2Cx, uint8_t EnOrDi)
{
    if(EnOrDi)
    {
        pI2Cx->CR1 |= (1 << 0); // Bit PE (Peripheral Enable)
    }else
    {
        pI2Cx->CR1 &= ~(1 << 0);
    }
}

/* 2. Hàm kh?i t?o I2C (Main g?i cái này b? l?i) */
void MCAL_I2C_Init(I2C_Handle_t *pI2CHandle)
{
    uint32_t tempreg = 0;
    
    // a. C?p Clock cho I2C
    MCAL_RCC_I2C_ClockControl(pI2CHandle->pI2Cx, 1);

    // b. C?u hình ACK (Thanh ghi CR1)
    tempreg = 0;
    if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
    {
        tempreg |= (1 << 10); // Bit 10: ACK
    }
    pI2CHandle->pI2Cx->CR1 = tempreg;

    // c. C?u hình T?n s? (Thanh ghi CR2 - FREQ)
    // Gi? s? APB1 = 16MHz
    uint32_t pclk1 = 16; 
    
    tempreg = 0;
    tempreg |= (pclk1 & 0x3F); // L?y 6 bit d?u
    pI2CHandle->pI2Cx->CR2 = tempreg;

    // d. C?u hình d?a ch? Slave (OAR1)
    tempreg = 0;
    tempreg |= (1 << 14); // Bit 14 luôn ph?i gi? là 1
    tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);
    pI2CHandle->pI2Cx->OAR1 = tempreg;

    // e. C?u hình Clock Control (CCR)
    uint16_t ccr_value = 0;
    tempreg = 0;

    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        // STANDARD MODE (100kHz)
        ccr_value = (pclk1 * 1000000U) / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        tempreg |= (ccr_value & 0xFFF);
    }
    else
    {
        // FAST MODE (400kHz)
        tempreg |= (1 << 15);
        tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
        
        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
        {
            ccr_value = (pclk1 * 1000000U) / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        }
        else
        {
            ccr_value = (pclk1 * 1000000U) / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        }
        tempreg |= (ccr_value & 0xFFF);
    }
    pI2CHandle->pI2Cx->CCR = tempreg;

    // f. C?u hình TRISE
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        tempreg = (pclk1 + 1);
    }
    else
    {
        tempreg = ((pclk1 * 300) / 1000 + 1);
    }
    pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}

/* 3. Hàm g?i d? li?u (M?i thêm) */
void MCAL_I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
    I2C_Type_Def *pI2Cx = pI2CHandle->pI2Cx;

    // 1. T?o di?u ki?n START
    pI2Cx->CR1 |= (1 << 8); 
    
    // 2. Ch? Start bit du?c t?o xong (SB=1)
    while( ! (pI2Cx->SR1 & (1 << 0)) );

    // 3. G?i d?a ch? Slave + Bit Write (0)
    // Fix warning implicit conversion: ép ki?u uint8_t
    SlaveAddr = (uint8_t)(SlaveAddr << 1);
    SlaveAddr &= ~(1); 
    pI2Cx->DR = SlaveAddr;

    // 4. Ch? g?i d?a ch? xong (ADDR=1)
    while( ! (pI2Cx->SR1 & (1 << 1)) );

    // 5. Xóa c? ADDR (Ð?c SR1 r?i SR2)
    uint32_t dummyRead = pI2Cx->SR1;
    dummyRead = pI2Cx->SR2;
    (void)dummyRead; 

    // 6. G?i d? li?u t?ng Byte
    while(Len > 0)
    {
        // Ch? d?m truy?n tr?ng (TXE=1)
        while( ! (pI2Cx->SR1 & (1 << 7)) );
        
        pI2Cx->DR = *pTxBuffer;
        
        pTxBuffer++;
        Len--;
    }

    // 7. Ch? TXE=1 và BTF=1
    while( ! (pI2Cx->SR1 & (1 << 7)) ); // TXE
    while( ! (pI2Cx->SR1 & (1 << 2)) ); // BTF

    // 8. T?o di?u ki?n STOP
    pI2Cx->CR1 |= (1 << 9); 
}