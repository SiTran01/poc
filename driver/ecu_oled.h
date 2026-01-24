#ifndef ECU_OLED_H
#define ECU_OLED_H

#include "stm32f407_base.h"
#include "mcal_i2c.h" // Đảm bảo file này có định nghĩa I2C_Handle_t
#include "mcal_gpio.h" // Cần để config GPIO
#include "oled_font.h"

/* Cấu hình địa chỉ I2C */
#define OLED_I2C_ADDR        0x3C
#define OLED_WIDTH           128
#define OLED_HEIGHT          64
#define OLED_COLOR_BLACK     0
#define OLED_COLOR_WHITE     1

/* Struct quản lý OLED */
typedef struct {
    // Thay đổi: Chứa trực tiếp biến Handle, không dùng con trỏ trỏ ra ngoài nữa
    I2C_Handle_t I2C_Handle; 
} OLED_Handle_t;

/* --- API --- */
/* Thay đổi hàm Init: Nhận thông tin phần cứng thay vì nhận Handle I2C */
void ECU_OLED_Init(OLED_Handle_t *pOled, I2C_Type_Def *I2Cx, 
                   GPIO_Type_Def *GPIO_Port, uint8_t SCL_Pin, uint8_t SDA_Pin, uint8_t AF_Mode);

void ECU_OLED_Clear(OLED_Handle_t *pOled);
void ECU_OLED_UpdateScreen(OLED_Handle_t *pOled);
void ECU_OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

// Hàm in Font chuẩn 5x7
void ECU_OLED_PrintChar_F5x7(uint8_t x, uint8_t y, char c);
void ECU_OLED_PrintString_F5x7(uint8_t x, uint8_t y, char *str);

// [MỚI] Hàm in Font nhỏ 3x5
void ECU_OLED_PrintChar_F3x5(uint8_t x, uint8_t y, char c);
void ECU_OLED_PrintString_F3x5(uint8_t x, uint8_t y, char *str);

void ECU_OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);

// Vẽ nửa vòng tròn (Dùng cho khung đồng hồ)
// x0, y0: Tọa độ tâm
// r: Bán kính
void ECU_OLED_DrawSemiCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

// Vẽ nguyên cái đồng hồ tốc độ (Hàm cấp cao)
// speed: Tốc độ từ 0 đến 100
void ECU_OLED_DrawSpeedometer(uint8_t speed);

#endif