#include "mcal_adc.h"
#include "mcal_rcc.h"

void MCAL_ADC_Init(ADC_Handle_t *pADCHandle)
{
    ADC_Type_Def *pADCx = pADCHandle->pADCx;

    // 1. Cấp Clock cho ADC
    MCAL_RCC_ADC_ClockControl(pADCx, 1);

    // 2. Cấu hình Prescaler (ADC_COMMON)
    // Clock ADC < 36MHz. APB2=84MHz -> Chia 4 = 21MHz.
    ADC_COMMON->CCR &= ~(3U << 16); 
    ADC_COMMON->CCR |= (1U << 16);  // Div 4

    // 3. [FIX QUAN TRỌNG] Cấu hình Sampling Time (Thời gian lấy mẫu)
    // Biến trở thường có trở kháng cao, cần thời gian nạp tụ lâu.
    // Ta chọn mức Max: 480 Cycles (Bit '111' = 7) cho an toàn tuyệt đối.
    
    uint8_t channel = pADCHandle->Channel;
    
    if (channel < 10) 
    {
        // Channel 0 - 9 nằm ở thanh ghi SMPR2
        // Mỗi channel chiếm 3 bit.
        pADCx->SMPR2 &= ~(7U << (3 * channel)); // Xóa 3 bit cũ
        pADCx->SMPR2 |= (7U << (3 * channel));  // Set 111 (480 Cycles)
    }
    else 
    {
        // Channel 10 - 18 nằm ở thanh ghi SMPR1
        // Channel 10 bắt đầu từ bit 0 của SMPR1
        pADCx->SMPR1 &= ~(7U << (3 * (channel - 10))); // Xóa 3 bit cũ
        pADCx->SMPR1 |= (7U << (3 * (channel - 10)));  // Set 111
    }

    // 4. Cấu hình Sequence (Thứ tự chuyển đổi)
    // Đặt Channel của ông vào vị trí số 1 (Rank 1)
    pADCx->SQR3 &= ~(0x1FU << 0);
    pADCx->SQR3 |= (channel << 0);

    // 5. Bật ADC
    pADCx->CR2 |= (1U << 0);
    
    // Delay nhẹ để ADC khởi động nguồn ổn định
    for(int i=0; i<10000; i++);
}

uint16_t MCAL_ADC_Read(ADC_Handle_t *pADCHandle)
{
    ADC_Type_Def *pADCx = pADCHandle->pADCx;

    // Start Conversion (SWSTART)
    pADCx->CR2 |= (1U << 30);

    // Wait for EOC (End of Conversion)
    while(!(pADCx->SR & (1U << 1)));

    // Read Data
    return (uint16_t)(pADCx->DR);
}
