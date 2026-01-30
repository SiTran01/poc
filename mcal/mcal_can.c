/* FILE: mcal_can.c - ÐÃ FIX L?I FILTER CAN2 */
#include "mcal_can.h"
#include "mcal_rcc.h" 

void MCAL_CAN_Init(CAN_TypeDef *pCANx) {
    // 1. C?p clock (Ðo?n này b?n dã làm dúng, tôi gi? nguyên)
    if(pCANx == CAN1) {
        RCC->APB1ENR |= (1 << 25); 
    }
    else if (pCANx == CAN2) {
        RCC->APB1ENR |= (1 << 25); // CAN2 c?n CAN1 clock (Master)
        RCC->APB1ENR |= (1 << 26); // B?t Clock CAN2
    }
    
    // 2. Vào ch? d? Initialization
    pCANx->MCR |= (1 << 0);
    while(!(pCANx->MSR & (1 << 0))); 

    // 3. Thoát ch? d? Sleep
    pCANx->MCR &= ~(1 << 1);
    while(pCANx->MSR & (1 << 1)); 

    // 4. C?u hình MCR
    pCANx->MCR |= (1 << 6); // ABOM
    pCANx->MCR |= (1 << 2); // TXFP

    /* 5. C?u hình Baudrate 500kbps */
    pCANx->BTR = 0;
    pCANx->BTR |= (5 << 0);   
    pCANx->BTR |= (10 << 16); 
    pCANx->BTR |= (1 << 20);  
    pCANx->BTR |= (0 << 24);  
    
    // COMMENT DÒNG NÀY KHI CH?Y TH?T
    //pCANx->BTR |= (1 << 30); 
}

void MCAL_CAN_Start(CAN_TypeDef *pCANx) {
    pCANx->MCR &= ~(1 << 0); 
    while(pCANx->MSR & (1 << 0)); 
}

/* --- [QUAN TR?NG] HÀM NÀY ÐÃ ÐU?C VI?T L?I HOÀN TOÀN --- */
void MCAL_CAN_ConfigFilter(CAN_TypeDef *pCANx) {
    // ?? QUAN TR?NG: Luôn dùng CAN1 d? di?u khi?n b? l?c
    CAN_TypeDef *pMasterCAN = CAN1; 
    
    uint32_t filterBank;
    
    // N?u là CAN1: Dùng Bank 0. N?u là CAN2: Dùng Bank 14
    if(pCANx == CAN1) {
        filterBank = 0;
    } else {
        filterBank = 14; // CAN2 dùng t? Bank 14 tr? di
    }

    // 1. Vào ch? d? Init Filter (Dùng CAN1)
    pMasterCAN->FMR |= (1 << 0); 

    // 2. Chia d?a bàn: Bank 0-13 cho CAN1, 14-27 cho CAN2
    pMasterCAN->FMR &= ~(0x3F << 8);
    pMasterCAN->FMR |= (14 << 8);

    pMasterCAN->FA1R &= ~(1 << filterBank); 
    
    pMasterCAN->FS1R |= (1 << filterBank);  
    pMasterCAN->FM1R &= ~(1 << filterBank); 
    
    pMasterCAN->sFilterRegister[filterBank].FR1 = 0;
    pMasterCAN->sFilterRegister[filterBank].FR2 = 0;

    pMasterCAN->FFA1R &= ~(1 << filterBank); 

    pMasterCAN->FA1R |= (1 << filterBank);

    pMasterCAN->FMR &= ~(1 << 0);
}

uint8_t MCAL_CAN_Transmit(CAN_TypeDef *pCANx, CAN_TxHeader_t *pHeader) {
    uint8_t mailbox;
    if ((pCANx->TSR & (1<<26))) mailbox = 0;
    else if ((pCANx->TSR & (1<<27))) mailbox = 1;
    else if ((pCANx->TSR & (1<<28))) mailbox = 2;
    else return 0; 

    pCANx->sTxMailBox[mailbox].TIR = (pHeader->StdId << 21);
    pCANx->sTxMailBox[mailbox].TDTR = (pHeader->DLC & 0x0F);
    pCANx->sTxMailBox[mailbox].TDLR = (pHeader->Data[3] << 24) | (pHeader->Data[2] << 16) | 
                                      (pHeader->Data[1] << 8)  | (pHeader->Data[0]);
    pCANx->sTxMailBox[mailbox].TDHR = (pHeader->Data[7] << 24) | (pHeader->Data[6] << 16) | 
                                      (pHeader->Data[5] << 8)  | (pHeader->Data[4]);
    pCANx->sTxMailBox[mailbox].TIR |= (1 << 0);
    return 1;
}

uint8_t MCAL_CAN_Receive(CAN_TypeDef *pCANx, CAN_RxHeader_t *pHeader) {
    if ((pCANx->RF0R & 0x03) == 0) return 0; 

    pHeader->StdId = (pCANx->sFIFOMailBox[0].RIR >> 21);
    pHeader->DLC = (pCANx->sFIFOMailBox[0].RDTR & 0x0F);
    
    uint32_t low = pCANx->sFIFOMailBox[0].RDLR;
    uint32_t high = pCANx->sFIFOMailBox[0].RDHR;

    pHeader->Data[0] = (uint8_t)(low & 0xFF);
    pHeader->Data[1] = (uint8_t)((low >> 8) & 0xFF);
    pHeader->Data[2] = (uint8_t)((low >> 16) & 0xFF);
    pHeader->Data[3] = (uint8_t)((low >> 24) & 0xFF);
    pHeader->Data[4] = (uint8_t)(high & 0xFF);
    pHeader->Data[5] = (uint8_t)((high >> 8) & 0xFF);
    pHeader->Data[6] = (uint8_t)((high >> 16) & 0xFF);
    pHeader->Data[7] = (uint8_t)((high >> 24) & 0xFF);

    pCANx->RF0R |= (1 << 5); 
    return 1;
}

