#include "mcal_can.h"
#include "mcal_rcc.h"

void MCAL_CAN_Init(CAN_TypeDef *pCANx) {
    // 1. C?p clock cho b? CAN1/CAN2
    if(pCANx == CAN1) {
        RCC->APB1ENR |= (1 << 25); // CAN1EN
    }
    
    // 2. Vào ch? d? Initialization (INRQ = 1)
    pCANx->MCR |= (1 << 0);
    while(!(pCANx->MSR & (1 << 0))); // Ð?i INAK = 1

    // 3. Thoát ch? d? Sleep (SLEEP = 0)
    pCANx->MCR &= ~(1 << 1);
    while(pCANx->MSR & (1 << 1)); // Ð?i SLAK = 0

    // 4. C?u hình Master Control Register (MCR)
    pCANx->MCR |= (1 << 6); // ABOM: T? d?ng ph?c h?i khi l?i Bus-off
    pCANx->MCR |= (1 << 2); // TXFP: Uu tiên g?i theo th? t? Mailbox (FIFO)

    /* 5. C?u hình Bit Timing (BTR) cho 500kbps 
       Gi? s? APB1 Clock = 42MHz. Prescaler = 6 => CAN Clock = 7MHz
       Time Quanta (TQ) = 1/7MHz
       Baudrate = 1 / (TQ * (1 + TS1 + TS2)) 
       V?i TS1 = 11, TS2 = 2 => Total TQ = 14. 7MHz / 14 = 500kbps
    */
    pCANx->BTR = 0;
    pCANx->BTR |= (5 << 0);   // Prescaler = 6 (0..9 bit: 5+1)
    pCANx->BTR |= (10 << 16); // TS1 = 11 (16..19 bit: 10+1)
    pCANx->BTR |= (1 << 20);  // TS2 = 2 (20..22 bit: 1+1)
    pCANx->BTR |= (0 << 24);  // SJW = 1 (24..25 bit: 0+1)
		pCANx->BTR |= (1 << 30);  // Uncomment dòng này d? test m?t mình
		
}

void MCAL_CAN_Start(CAN_TypeDef *pCANx) {
    pCANx->MCR &= ~(1 << 0); // INRQ = 0 d? b?t d?u ch?y
    while(pCANx->MSR & (1 << 0)); // Ð?i INAK = 0
}

void MCAL_CAN_ConfigFilter(CAN_TypeDef *pCANx) {
    // Vào ch? d? c?u hình Filter
    pCANx->FMR |= (1 << 0); // FINIT = 1

    // Ch?n Filter Bank 0, cho phép nh?n m?i tin nh?n (Mask = 0)
    pCANx->FM1R &= ~(1 << 0); // Ch? d? Mask
    pCANx->FS1R |= (1 << 0);  // 32-bit Scale
    
    pCANx->sFilterRegister[0].FR1 = 0x00000000;
    pCANx->sFilterRegister[0].FR2 = 0x00000000;

    pCANx->FFA1R &= ~(1 << 0); // Gán cho FIFO 0
    pCANx->FA1R |= (1 << 0);   // Kích ho?t Filter 0

    pCANx->FMR &= ~(1 << 0); // FINIT = 0 thoát c?u hình
}

uint8_t MCAL_CAN_Transmit(CAN_TypeDef *pCANx, CAN_TxHeader_t *pHeader) {
    uint8_t mailbox;

    // Ki?m tra Mailbox tr?ng (TME0, TME1, TME2)
    if (pCANx->TSR & (1 << 26)) mailbox = 0;
    else if (pCANx->TSR & (1 << 27)) mailbox = 1;
    else if (pCANx->TSR & (1 << 28)) mailbox = 2;
    else return 0; // Bus busy

    // 1. Cài d?t ID (Standard ID)
    pCANx->sTxMailBox[mailbox].TIR = (pHeader->StdId << 21);

    // 2. Cài d?t DLC
    pCANx->sTxMailBox[mailbox].TDTR = (pHeader->DLC & 0x0F);

    // 3. Cài d?t d? li?u (Low và High Register)
    pCANx->sTxMailBox[mailbox].TDLR = (pHeader->Data[3] << 24) | (pHeader->Data[2] << 16) | 
                                      (pHeader->Data[1] << 8)  | (pHeader->Data[0]);
    pCANx->sTxMailBox[mailbox].TDHR = (pHeader->Data[7] << 24) | (pHeader->Data[6] << 16) | 
                                      (pHeader->Data[5] << 8)  | (pHeader->Data[4]);

    // 4. Yêu c?u g?i (TXRQ)
    pCANx->sTxMailBox[mailbox].TIR |= (1 << 0);

    return 1;
}

uint8_t MCAL_CAN_Receive(CAN_TypeDef *pCANx, CAN_RxHeader_t *pHeader) {
    // Ki?m tra FIFO 0 có tin nh?n không (FMP0 bits)
    if ((pCANx->RF0R & 0x03) == 0) return 0;

    // 1. L?y ID
    pHeader->StdId = (pCANx->sFIFOMailBox[0].RIR >> 21);

    // 2. L?y DLC
    pHeader->DLC = (pCANx->sFIFOMailBox[0].RDTR & 0x0F);

    // 3. L?y d? li?u
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

    // 4. Gi?i phóng FIFO 0 (Release)
    pCANx->RF0R |= (1 << 5); // RFOM0

    return 1;
}
