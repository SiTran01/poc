/* ecu_can.c */
#include "ecu_can.h"

void ECU_CAN_Init(CAN_Handle_t *pHandle, CAN_TypeDef *CANx, 
                  GPIO_Type_Def *GPIO_Port, uint8_t TxPin, uint8_t RxPin, uint8_t AF_Mode)
{
    if(pHandle == NULL || CANx == NULL || GPIO_Port == NULL) return;

    pHandle->pCANx = CANx;

    /* 1. C?u hình GPIO cho CAN (TX và RX) 
       - Mode: Alternate Function
       - AF: AF9 (thu?ng là v?y cho CAN1)
       - Speed: Very High
       - Output Type: Push Pull (CAN Controller t? lo vi?c lái bus)
       - Pull-up: Nên có cho chân RX d? tránh nhi?u khi bus r?nh
    */
    GPIO_Handle_t can_gpio;
    can_gpio.Port = GPIO_Port;
    
    can_gpio.Pinconfig.GPIO_PinMode      = GPIO_MODE_ALTFN;
    can_gpio.Pinconfig.GPIO_PinAltFunMode = AF_Mode;
    can_gpio.Pinconfig.GPIO_PinSpeed     = GPIO_SPEED_VERY_HIGH;
    can_gpio.Pinconfig.GPIO_OutputType   = GPIO_OUTPUT_TYPE_PP; 
    can_gpio.Pinconfig.GPIO_PUPD         = GPIO_PULL_UP;

    // Init TX
    can_gpio.Pinconfig.GPIO_PinNumber = TxPin;
    MCAL_GPIO_Init(&can_gpio);

    // Init RX
    can_gpio.Pinconfig.GPIO_PinNumber = RxPin;
    MCAL_GPIO_Init(&can_gpio);

    /* 2. G?i MCAL d? Init b? CAN Controller */
    MCAL_CAN_Init(CANx);

    /* 3. C?u hình Filter d? nh?n m?i tin (ho?c ch?nh l?i n?u mu?n l?c) */
    MCAL_CAN_ConfigFilter(CANx);

    /* 4. Start CAN */
    MCAL_CAN_Start(CANx);
}

uint8_t ECU_CAN_Send(CAN_Handle_t *pHandle, uint32_t ID, uint8_t *pData, uint8_t Len) {
    CAN_TxHeader_t txHeader;
    txHeader.StdId = ID;
    txHeader.DLC = Len;
    
    // Copy data
    for(int i=0; i<Len; i++) {
        txHeader.Data[i] = pData[i];
    }

    return MCAL_CAN_Transmit(pHandle->pCANx, &txHeader);
}

uint8_t ECU_CAN_Receive(CAN_Handle_t *pHandle, uint32_t *pID, uint8_t *pData, uint8_t *pLen) {
    CAN_RxHeader_t rxHeader;
    
    if(MCAL_CAN_Receive(pHandle->pCANx, &rxHeader)) {
        *pID = rxHeader.StdId;
        *pLen = rxHeader.DLC;
        for(int i=0; i<rxHeader.DLC; i++) {
            pData[i] = rxHeader.Data[i];
        }
        return 1; // Có tin m?i
    }
    return 0; // Không có tin
}
