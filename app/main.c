#include "system_manager.h"
#include "peri_init.h" 
#include "ecu_can.h"
#include "ecu_oled.h"
#include "ecu_pedal.h"   
#include "ecu_servo.h"   
#include "FreeRTOS.h"
#include "task.h"

//variable from peri
extern Pedal_Handle_t hPedal;
extern Servo_Handle_t hServo;
extern OLED_Handle_t  hOled;
extern CAN_Handle_t   hCAN;   // CAN 1 (trans)
extern CAN_Handle_t   hCAN2;  // CAN 2 (recieve)

volatile uint8_t  g_throttle_val = 0; 
volatile uint8_t  g_rx_val = 0;       
volatile uint16_t g_debug_adc_raw = 0; 

//task prototype
void vTask_Engine_Control(void *pvParam); 
void vTask_Dashboard_Rx(void *pvParam);   
void vTask_Display_OLED(void *pvParam);   

int main(void) {
    System_Init();
    PERI_Init(); 

    RCC->AHB1ENR |= (1 << 3); 
    GPIOD->MODER |= (1 << (13*2)) | (1 << (14*2)) | (1 << (15*2));

    xTaskCreate(vTask_Engine_Control, "Engine_Ctrl", 256, NULL, 3, NULL);
    xTaskCreate(vTask_Dashboard_Rx, "Dash_Rx", 256, NULL, 2, NULL);
    xTaskCreate(vTask_Display_OLED, "Display", 512, NULL, 1, NULL);

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
        
        // led 4 debug
        // 
        if(ECU_CAN_Send(&hCAN, 0x123, tx_data, 1)) {
            GPIOD->ODR ^= (1 << 13); 
            GPIOD->ODR &= ~(1 << 14);
        } else {
            GPIOD->ODR |= (1 << 14); 
        }

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
                
                // --- LOGIC ĐÈN RX (PD15 - XANH) ---
                
                GPIOD->ODR ^= (1 << 15); 
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/* ==============================================================================
   TASK 3: OLED DISPLAY
   ============================================================================== */
void vTask_Display_OLED(void *pvParam) {
    char buff[20];
    uint8_t old_rx_val = 255;
    uint8_t old_throttle = 255;

    ECU_OLED_Clear(&hOled);
    ECU_OLED_UpdateScreen(&hOled);

    while(1) {
        if(g_rx_val != old_rx_val || g_throttle_val != old_throttle) {
            old_rx_val = g_rx_val;
            old_throttle = g_throttle_val;

            ECU_OLED_Clear(&hOled);
            
            //
            sprintf(buff, "GAS:%d%% RAW:%d", g_throttle_val, g_debug_adc_raw);
            ECU_OLED_PrintString_F5x7(0, 0, buff);
            
            // Đồng hồ: Hiển thị giá trị Nhận (CAN2 Rx)
            ECU_OLED_DrawSpeedometer(g_rx_val);
            
            // Nếu nhận được data (>0), báo RX:OK
            if(g_rx_val > 0) ECU_OLED_PrintString_F5x7(90, 20, "RX:OK");
            else             ECU_OLED_PrintString_F5x7(90, 20, "WAIT...");

            ECU_OLED_UpdateScreen(&hOled);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}