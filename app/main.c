#include "system_manager.h"
#include "peri_init.h" 
#include "ecu_can.h"
#include "ecu_oled.h"
#include "ecu_pedal.h"    
#include "ecu_servo.h"    
#include "ecu_led.h"  
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h> // [QUAN TRỌNG] Thêm thư viện này để dùng hàm abs()

//variable from peri
extern Pedal_Handle_t hPedal;
extern Servo_Handle_t hServo;
extern OLED_Handle_t  hOled;
extern CAN_Handle_t   hCAN;   // CAN 1 (trans)
extern CAN_Handle_t   hCAN2;  // CAN 2 (recieve)
extern LED_Handle_t   hGreenLed;

volatile uint8_t  g_throttle_val = 0; 
volatile uint8_t  g_rx_val = 0;       
volatile uint16_t g_debug_adc_raw = 0; 

//task prototype
void vTask_Engine_Control(void *pvParam); 
void vTask_Dashboard_Rx(void *pvParam);   
void vTask_Display_OLED(void *pvParam);   
void vTask_Blink_LED (void *pvParam);

int main(void) {
    System_Init();
    PERI_Init(); 

    xTaskCreate(vTask_Engine_Control, "Engine_Ctrl", 256, NULL, 3, NULL);
    xTaskCreate(vTask_Dashboard_Rx, "Dash_Rx", 256, NULL, 2, NULL);
    xTaskCreate(vTask_Display_OLED, "Display", 512, NULL, 1, NULL);
    
    // [FIX 1] Tăng Stack Size từ 64 lên 128 (Cho an toàn)
    xTaskCreate(vTask_Blink_LED, "Blink", 128, NULL, 4, NULL);

    vTaskStartScheduler();
    while(1);
}

/* ==============================================================================
   TASK 1: GỬI TIN (ENGINE - CAN 1)
   ============================================================================== */
void vTask_Engine_Control(void *pvParam) {
    uint8_t pedal_percent = 0;
    uint8_t servo_angle = 0;
    uint8_t tx_data[1];
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        // read pedal - control serrvo
        pedal_percent = ECU_Pedal_GetPosition(&hPedal);
        g_throttle_val = pedal_percent;
        g_debug_adc_raw = ADC1->DR; 

        servo_angle = (uint8_t)((pedal_percent * 180) / 100);
        ECU_Servo_WriteAngle(&hServo, servo_angle);

        // send data via CAN 1 (&hCAN)
        tx_data[0] = pedal_percent;
        
        ECU_CAN_Send(&hCAN, 0x123, tx_data, 1);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(15)); 
    }
}

/* ==============================================================================
   TASK 2: NHẬN TIN (DASHBOARD - CAN 2)
   ============================================================================== */
void vTask_Dashboard_Rx(void *pvParam) {
    uint32_t rx_id;
    uint8_t rx_data[8];
    uint8_t rx_len;

    while(1) {
        if(ECU_CAN_Receive(&hCAN2, &rx_id, rx_data, &rx_len)) { 
            if(rx_id == 0x123) {
                g_rx_val = rx_data[0]; 
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* ==============================================================================
   TASK 3: OLED DISPLAY (Đã tối ưu)
   ============================================================================== */
void vTask_Display_OLED(void *pvParam) {
    char buff[20];
    uint8_t old_rx_val = 255;
    uint8_t old_throttle = 255;

    ECU_OLED_Clear(&hOled);
    ECU_OLED_UpdateScreen(&hOled);

    while(1) {
        // [FIX 2] LỌC NHIỄU ADC: Chỉ vẽ khi thay đổi > 2 đơn vị
        if(g_rx_val != old_rx_val || abs((int)g_throttle_val - (int)old_throttle) > 2) {
            
            old_rx_val = g_rx_val;
            old_throttle = g_throttle_val;

            ECU_OLED_Clear(&hOled);
            
            // Vẽ thông số
            sprintf(buff, "GAS:%d%% RAW:%d", g_throttle_val, g_debug_adc_raw);
            ECU_OLED_PrintString_F5x7(0, 0, buff);
            
            // Đồng hồ
            ECU_OLED_DrawSpeedometer(g_rx_val);
            
            // Trạng thái
            if(g_rx_val > 0) ECU_OLED_PrintString_F5x7(90, 20, "RX:OK");
            else             ECU_OLED_PrintString_F5x7(90, 20, "WAIT...");

            ECU_OLED_UpdateScreen(&hOled);
        }

        // [FIX 3] Giảm Delay xuống 20ms cho mượt (vì Driver mới đã nhanh rồi)
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void vTask_Blink_LED(void *pvParam){
    while(1){
        ECU_LED_Toggle(&hGreenLed);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}