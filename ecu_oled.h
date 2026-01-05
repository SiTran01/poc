#ifndef ECU_OLED_H
#define ECU_OLED_H

#include "stm32f407_base.h"
#include "mcal_i2c.h"
#include "oled_font.h" // [Quan trọng] Include file font tách riêng

/* Cấu hình địa chỉ I2C */
#define OLED_I2C_ADDR        0x3C

/* Kích thước màn hình */
#define OLED_WIDTH           128
#define OLED_HEIGHT          64

/* Màu sắc */
#define OLED_COLOR_BLACK     0
#define OLED_COLOR_WHITE     1

/* Struct quản lý OLED */
typedef struct {
    I2C_Handle_t *pI2CHandle;
} OLED_Handle_t;

/* --- API --- */
void ECU_OLED_Init(OLED_Handle_t *pOled, I2C_Handle_t *pI2C);
void ECU_OLED_Clear(void);
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