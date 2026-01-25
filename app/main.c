#include "stm32f407_base.h"
#include "system_manager.h"
#include "mcal_systick.h"
#include "peri_init.h"      // Quan trọng: Chứa extern hPedal, hServo, hOled, hCAN
#include "oled_font.h"      // Để dùng font chữ

/* --- DEFINES --- */
#define ADC_FILTER_SIZE     8
#define PEDAL_MIN_ADC       50
#define PEDAL_MAX_ADC       4000
#define CAN_TX_ID_SPEED     0x123 // ID của gói tin tốc độ

/* --- GLOBAL VARS --- */
uint16_t adc_raw, adc_flt;
uint8_t pedal_percent, servo_angle;

/* --- HELPER FUNCTIONS --- */
uint16_t Filter_ADC(uint16_t new_sample) {
    static uint16_t buffer[ADC_FILTER_SIZE];
    static uint32_t sum = 0;
    static uint8_t index = 0;
    
    sum -= buffer[index];
    buffer[index] = new_sample;
    sum += new_sample;
    index++;
    
    if(index >= ADC_FILTER_SIZE) index = 0;
    return (uint16_t)(sum / ADC_FILTER_SIZE);
}

/* --- MAIN FUNCTION --- */
int main(void) {
    // 1. Khởi tạo Clock hệ thống (HSE/HSI -> PLL -> System Clock)
    System_Init();

    // 2. Khởi tạo Systick (Để dùng hàm MCAL_Delay_ms)
    // Giả sử SystemCoreClock là 16MHz (hoặc tốc độ mày cấu hình trong System_Init)
    // Nếu hàm System_Init đã gọi Systick rồi thì dòng này có thể bỏ
//    MCAL_SysTick_Init(16000000); 

    // 3. Khởi tạo ngoại vi (Pedal, Servo, OLED, CAN)
    // Hàm này nằm trong peri_init.c -> Gọi ECU_..._Init -> Gọi MCAL_..._Init
    PERI_Init();
		uint8_t rx_speed = 0;
    while (1) {
        // [FIX 1] Xóa màn hình ngay đầu vòng lặp để chuẩn bị vẽ frame mới
        ECU_OLED_Clear(&hOled); 

        // --- B1: ĐỌC ADC & TÍNH TOÁN GỬI ĐI ---
        adc_raw = MCAL_ADC_Read(&hPedal.ADCHandle);
        adc_flt = Filter_ADC(adc_raw);
        if (adc_flt < PEDAL_MIN_ADC) adc_flt = PEDAL_MIN_ADC;
        else if (adc_flt > PEDAL_MAX_ADC) adc_flt = PEDAL_MAX_ADC;
        
        pedal_percent = (uint8_t)(((uint32_t)(adc_flt - PEDAL_MIN_ADC) * 100) / (PEDAL_MAX_ADC - PEDAL_MIN_ADC));
        servo_angle = (uint8_t)(((uint16_t)pedal_percent * 180) / 100);

        ECU_Servo_WriteAngle(&hServo, servo_angle);

        // --- B2: GỬI CAN (TX) ---
        uint8_t tx_data[1];
        tx_data[0] = pedal_percent;
        ECU_CAN_Send(&hCAN, 0x123, tx_data, 1);

        // --- B3: NHẬN CAN (RX) ---
        uint32_t rx_id = 0;
        uint8_t  rx_data[8] = {0};
        uint8_t  rx_len = 0;

        if(ECU_CAN_Receive(&hCAN, &rx_id, rx_data, &rx_len)) {
            if(rx_id == 0x123) {
                // [FIX 2] Cập nhật dữ liệu từ CAN vào biến hiển thị
                rx_speed = rx_data[0]; 
                
                // In trạng thái (Lúc này vẽ lên buffer sạch nên không bị xóa)
                ECU_OLED_PrintString_F5x7(80, 0, "LB:OK  "); 
            } else {
                ECU_OLED_PrintString_F5x7(80, 0, "LB:ERR ");
            }
        } else {
            ECU_OLED_PrintString_F5x7(80, 0, "NO RX  ");
        }

        // --- B4: HIỂN THỊ ĐỒNG HỒ ---
        ECU_OLED_PrintString_F3x5(55, 0, "SPEED");
        
        // [FIX 3] Vẽ đồng hồ bằng dữ liệu nhận từ CAN (rx_speed)
        // Chứ KHÔNG vẽ bằng pedal_percent (dữ liệu gốc)
        ECU_OLED_DrawSpeedometer(rx_speed); 
        
        // --- B5: ĐẨY RA MÀN HÌNH ---
        ECU_OLED_UpdateScreen(&hOled);

        MCAL_Delay_ms(50); 
    }
}

