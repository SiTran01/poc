#include "stm32f407_base.h"
#include "mcal_systick.h"
#include "ecu_pedal.h"
#include "system_manager.h"
#include "ecu_servo.h"
#include "ecu_oled.h"
#include "oled_font.h"
#include "peri_init.h"
// #include "mcal_i2c.h" // Không cần thiết phải include ở main nữa nếu ecu_oled.h đã có

#define ADC_FILTER_SIZE 8

// --- HÀM LỌC NHIỄU ADC GIỮ NGUYÊN ---
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

#define PEDAL_MIN_ADC 50
#define PEDAL_MAX_ADC 4000

uint16_t adc_raw, adc_flt;
uint8_t pedal_percent, servo_angle;

// Global Handles
//Servo_Handle_t hServo;
//OLED_Handle_t hOled; 
// Lưu ý: Không cần khai báo I2C_Handle_t hI2C ở đây nữa vì nó nằm trong hOled rồi

int main(void) {
    // 1. Khởi tạo Clock hệ thống
    System_Init();
		PERI_Init();

    // 2. Khởi tạo Pedal (ADC1, CH1, PA1)
//    Pedal_Handle_t hPedal;
//    ECU_Pedal_Init(&hPedal, ADC1, 1, GPIOA, 1);

//    // 3. Khởi tạo Servo (TIM4, CH1, PD12, AF2)
//    ECU_Servo_Init(&hServo, TIM4, 1, GPIOD, 12, 2);

//    /* 4. KHỞI TẠO OLED (ALL-IN-ONE)
//       - Dùng I2C1
//       - Chân GPIO B (SCL=Pin6, SDA=Pin7)
//       - Alternate Function Mode = 4 (AF4 là I2C1/2/3 trên F407)
//    */
//    ECU_OLED_Init(&hOled, I2C1, GPIOB, 6, 7, 4);

    while (1) {
        // --- B1: ĐỌC & XỬ LÝ ADC ---
        adc_raw = MCAL_ADC_Read(&hPedal.ADCHandle);
        adc_flt = Filter_ADC(adc_raw);

        // Map ADC sang %
        if (adc_flt < PEDAL_MIN_ADC) adc_flt = PEDAL_MIN_ADC;
        else if (adc_flt > PEDAL_MAX_ADC) adc_flt = PEDAL_MAX_ADC;
        
        pedal_percent = (uint8_t)(((uint32_t)(adc_flt - PEDAL_MIN_ADC) * 100) / (PEDAL_MAX_ADC - PEDAL_MIN_ADC));
        
        // Tính góc Servo
        servo_angle = (uint8_t)(((uint16_t)pedal_percent * 180) / 100);

        // --- B2: ĐIỀU KHIỂN SERVO ---
        ECU_Servo_WriteAngle(&hServo, servo_angle);

        // --- B3: HIỂN THỊ OLED ---
        ECU_OLED_Clear(&hOled);
        
        // In chữ font nhỏ 3x5
        ECU_OLED_PrintString_F3x5(55, 0, "SPEED"); 
        
        // Vẽ đồng hồ
        ECU_OLED_DrawSpeedometer(pedal_percent);
        
        // Đẩy buffer ra màn hình
        ECU_OLED_UpdateScreen(&hOled);

        // Delay nhẹ tránh màn hình bị nháy quá nhanh
        MCAL_Delay_ms(5); 
    }
}
