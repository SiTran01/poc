#include "ecu_oled.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h> // [QUAN TRỌNG] Thêm thư viện này để dùng memcmp, memcpy

// Bộ nhớ đệm màn hình hiện tại (New Frame)
static uint8_t OLED_Buffer[1024];

// [MỚI] Bộ nhớ lưu trạng thái cũ (Old Frame) để so sánh
static uint8_t OLED_Buffer_Prev[1024];

/* ============================================================
   PRIVATE FUNCTIONS
   ============================================================ */
static void OLED_WriteCommand(OLED_Handle_t *pOled, uint8_t cmd) {
    uint8_t data[2];
    data[0] = 0x00; // Control Byte: 0x00 = Command Stream
    data[1] = cmd;  // Command Byte
    
    // Gửi 2 byte qua I2C
    MCAL_I2C_MasterSendData(&pOled->I2C_Handle, data, 2, OLED_I2C_ADDR);
}

/* ============================================================
   INIT FUNCTION
   ============================================================ */
void ECU_OLED_Init(OLED_Handle_t *pOled, I2C_Type_Def *I2Cx, 
                   GPIO_Type_Def *GPIO_Port, uint8_t SCL_Pin, uint8_t SDA_Pin, uint8_t AF_Mode) 
{
    if(pOled == NULL || I2Cx == NULL || GPIO_Port == NULL) return;

    GPIO_Handle_t i2c_gpio;
    i2c_gpio.Port = GPIO_Port;
    
    // Cấu hình GPIO
    i2c_gpio.Pinconfig.GPIO_PinMode       = GPIO_MODE_ALTFN;       
    i2c_gpio.Pinconfig.GPIO_PinAltFunMode = AF_Mode;         
    i2c_gpio.Pinconfig.GPIO_OutputType    = GPIO_OUTPUT_TYPE_OD; 
    i2c_gpio.Pinconfig.GPIO_PinSpeed      = GPIO_SPEED_HIGH;
    i2c_gpio.Pinconfig.GPIO_PUPD          = GPIO_PULL_UP; 

    // Init chân SCL
    i2c_gpio.Pinconfig.GPIO_PinNumber = SCL_Pin;
    MCAL_GPIO_Init(&i2c_gpio);

    // Init chân SDA
    i2c_gpio.Pinconfig.GPIO_PinNumber = SDA_Pin;
    MCAL_GPIO_Init(&i2c_gpio);

    /* -------------------------------------------
       Cấu hình I2C Peripheral
       ------------------------------------------- */
    pOled->I2C_Handle.pI2Cx = I2Cx;
    
    // Cấu hình I2C Fast Mode (400kHz)
    pOled->I2C_Handle.I2C_Config.I2C_SCLSpeed      = 400000;
    pOled->I2C_Handle.I2C_Config.I2C_DeviceAddress = 0; 
    pOled->I2C_Handle.I2C_Config.I2C_AckControl    = I2C_ACK_ENABLE; 
    pOled->I2C_Handle.I2C_Config.I2C_FMDutyCycle   = I2C_FM_DUTY_2; 

    MCAL_I2C_Init(&pOled->I2C_Handle);
    MCAL_I2C_PeripheralControl(I2Cx, 1); 

    /* -------------------------------------------
       Gửi chuỗi lệnh khởi tạo màn hình
       ------------------------------------------- */
    OLED_WriteCommand(pOled, 0xAE); // Tắt màn hình
    OLED_WriteCommand(pOled, 0x20); 
    OLED_WriteCommand(pOled, 0x10); 
    OLED_WriteCommand(pOled, 0xB0); 
    OLED_WriteCommand(pOled, 0xC8); 
    OLED_WriteCommand(pOled, 0x00); 
    OLED_WriteCommand(pOled, 0x10); 
    OLED_WriteCommand(pOled, 0x40); 
    OLED_WriteCommand(pOled, 0x81); 
    OLED_WriteCommand(pOled, 0xFF);
    OLED_WriteCommand(pOled, 0xA1); 
    OLED_WriteCommand(pOled, 0xA6); 
    OLED_WriteCommand(pOled, 0xA8); 
    OLED_WriteCommand(pOled, 0x3F);
    OLED_WriteCommand(pOled, 0xA4); 
    OLED_WriteCommand(pOled, 0xD3); 
    OLED_WriteCommand(pOled, 0x00);
    OLED_WriteCommand(pOled, 0xD5); 
    OLED_WriteCommand(pOled, 0xF0);
    OLED_WriteCommand(pOled, 0xD9); 
    OLED_WriteCommand(pOled, 0x22);
    OLED_WriteCommand(pOled, 0xDA); 
    OLED_WriteCommand(pOled, 0x12);
    OLED_WriteCommand(pOled, 0xDB); 
    OLED_WriteCommand(pOled, 0x20);
    OLED_WriteCommand(pOled, 0x8D); 
    OLED_WriteCommand(pOled, 0x14); 
    OLED_WriteCommand(pOled, 0xAF); // Bật màn hình

    // [MỚI] Xóa sạch cả 2 buffer để đồng bộ dữ liệu ban đầu
    memset(OLED_Buffer, 0, 1024);
    memset(OLED_Buffer_Prev, 0, 1024);
    
    // Cập nhật màn hình đen lần đầu (Gửi hết)
    ECU_OLED_UpdateScreen(pOled);
}

/* ============================================================
   PUBLIC FUNCTIONS - DRAWING BASICS
   ============================================================ */

void ECU_OLED_Clear(OLED_Handle_t *pOled) {
    (void)pOled; 
    // Dùng memset cho nhanh gọn
    memset(OLED_Buffer, 0, 1024);
}

void ECU_OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if(x >= OLED_WIDTH || y >= OLED_HEIGHT) return;

    if(color == OLED_COLOR_WHITE) {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y % 8));
    } else {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}

// [HÀM QUAN TRỌNG NHẤT - ĐÃ TỐI ƯU SMART UPDATE]
void ECU_OLED_UpdateScreen(OLED_Handle_t *pOled) {
    uint8_t i2c_buff[129]; // 1 byte Control + 128 byte Data
    uint16_t offset;

    // Quét qua 8 Page
    for(uint8_t page = 0; page < 8; page++) {
        offset = page * 128; // Tính vị trí đầu của Page trong mảng 1024 byte

        // --- BƯỚC 1: SO SÁNH (Compare) ---
        // Nếu dữ liệu trang này ở Buffer Mới GIỐNG Y HỆT Buffer Cũ -> Bỏ qua (Skip)
        // Hàm memcmp trả về 0 nếu 2 vùng nhớ giống nhau
        if (memcmp(&OLED_Buffer[offset], &OLED_Buffer_Prev[offset], 128) == 0) {
            continue; // Nhảy sang Page tiếp theo, không gửi I2C -> Tiết kiệm thời gian!
        }

        // --- BƯỚC 2: NẾU KHÁC -> GỬI I2C ---
        OLED_WriteCommand(pOled, 0xB0 + page); // Set Page Start Address
        OLED_WriteCommand(pOled, 0x02);        // Low Column Address (SH1106 Offset)
        OLED_WriteCommand(pOled, 0x10);        // High Column Address

        // Chuẩn bị gói DATA
        i2c_buff[0] = 0x40; // Control Byte: Data Stream
        
        // Copy 128 byte từ Buffer Mới vào gói tin I2C
        memcpy(&i2c_buff[1], &OLED_Buffer[offset], 128);

        // Gửi gói tin qua I2C
        MCAL_I2C_MasterSendData(&pOled->I2C_Handle, i2c_buff, 129, OLED_I2C_ADDR);
        
        // --- BƯỚC 3: CẬP NHẬT TRẠNG THÁI CŨ ---
        // Copy dữ liệu vừa gửi vào Buffer Cũ để lần sau so sánh tiếp
        memcpy(&OLED_Buffer_Prev[offset], &OLED_Buffer[offset], 128);
    }
}

/* ============================================================
   PUBLIC FUNCTIONS - TEXT (FONTS)
   ============================================================ */
// Giữ nguyên logic cũ
void ECU_OLED_PrintChar_F5x7(uint8_t x, uint8_t y, char c) {
    if(c < 32 || c > 126) c = '?'; 
    
    for(uint8_t i = 0; i < OLED_FONT_WIDTH; i++) {
        uint8_t char_byte = Font5x7[c - 32][i]; 
        for(uint8_t j = 0; j < 8; j++) {
            if((char_byte >> j) & 0x01) ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_WHITE);
            else                        ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_BLACK);
        }
    }
}

void ECU_OLED_PrintString_F5x7(uint8_t x, uint8_t y, char *str) {
    while(*str) {
        if(x + OLED_FONT_WIDTH >= OLED_WIDTH) { 
            x = 0; y += 8;
        }
        ECU_OLED_PrintChar_F5x7(x, y, *str);
        x += (OLED_FONT_WIDTH + 1); 
        str++;
    }
}

void ECU_OLED_PrintChar_F3x5(uint8_t x, uint8_t y, char c) {
    if(c < 32 || c > 126) c = '?';
    
    for(uint8_t i = 0; i < OLED_FONT_SMALL_WIDTH; i++) { 
        uint8_t b = Font3x5[c - 32][i];
        for(uint8_t j = 0; j < 8; j++) {
            if((b >> j) & 0x01) ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_WHITE);
            else                ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_BLACK);
        }
    }
}

void ECU_OLED_PrintString_F3x5(uint8_t x, uint8_t y, char *str) {
    while(*str) {
        if(x + 4 >= OLED_WIDTH) { 
            x = 0; y += 8;
        }
        ECU_OLED_PrintChar_F3x5(x, y, *str);
        x += (OLED_FONT_SMALL_WIDTH + 1); 
        str++;
    }
}

/* ============================================================
   PUBLIC FUNCTIONS - GRAPHICS PRIMITIVES
   ============================================================ */

void ECU_OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;

    ECU_OLED_DrawPixel(x2, y2, color);
    
    while ((x1 != x2) || (y1 != y2)) {
        ECU_OLED_DrawPixel(x1, y1, color);
        error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

void ECU_OLED_DrawSemiCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
    int32_t f = 1 - r;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * r;
    int32_t x = 0;
    int32_t y = r;

    ECU_OLED_DrawPixel(x0 + r, y0, color);
    ECU_OLED_DrawPixel(x0 - r, y0, color);
    ECU_OLED_DrawPixel(x0, y0 - r, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ECU_OLED_DrawPixel(x0 + x, y0 - y, color);
        ECU_OLED_DrawPixel(x0 + y, y0 - x, color);
        ECU_OLED_DrawPixel(x0 - x, y0 - y, color);
        ECU_OLED_DrawPixel(x0 - y, y0 - x, color);
    }
}

/* ============================================================
   PUBLIC FUNCTIONS - APPLICATION SPECIFIC (SPEEDOMETER)
   ============================================================ */
#define PI 3.14159265

void ECU_OLED_DrawSpeedometer(uint8_t speed) {
    uint8_t cx = 64;
    uint8_t cy = 63; 
    uint8_t radius = 55; 

    ECU_OLED_DrawSemiCircle(cx, cy, radius, OLED_COLOR_WHITE);
    ECU_OLED_DrawSemiCircle(cx, cy, radius - 1, OLED_COLOR_WHITE);

    if(speed > 100) speed = 100;
    float angle = PI - ((float)speed / 100.0f) * PI;

    uint8_t needle_len = radius - 5;
    int16_t tip_x_temp = cx + (int16_t)(cos(angle) * needle_len);
    int16_t tip_y_temp = cy - (int16_t)(sin(angle) * needle_len); 

    if (tip_x_temp < 0) tip_x_temp = 0;
    if (tip_x_temp > 127) tip_x_temp = 127;
    if (tip_y_temp < 0) tip_y_temp = 0;
    if (tip_y_temp > 63) tip_y_temp = 63;

    ECU_OLED_DrawLine(cx, cy, (uint8_t)tip_x_temp, (uint8_t)tip_y_temp, OLED_COLOR_WHITE);

    char buff[5];
    sprintf(buff, "%d", speed);
    uint8_t text_x = 64 - (strlen(buff) * 6) / 2;
    ECU_OLED_PrintString_F5x7(text_x, cy - 10, buff);
}