#include "ecu_oled.h"

// Bộ nhớ đệm màn hình (Buffer) - 1KB RAM
static uint8_t OLED_Buffer[1024];

/* Hàm gửi lệnh xuống OLED (Internal) */
static void OLED_WriteCommand(OLED_Handle_t *pOled, uint8_t cmd) {
    uint8_t data[2];
    data[0] = 0x00; // Control Byte: Command
    data[1] = cmd;  // Lệnh cần gửi
    
    MCAL_I2C_MasterSendData(pOled->pI2CHandle, data, 2, OLED_I2C_ADDR);
}

/* Hàm khởi tạo */
void ECU_OLED_Init(OLED_Handle_t *pOled, I2C_Handle_t *pI2C) {
    pOled->pI2CHandle = pI2C;

    // Chuỗi lệnh khởi tạo chuẩn (Initialization Sequence)
    OLED_WriteCommand(pOled, 0xAE); // Tắt màn hình
    OLED_WriteCommand(pOled, 0x20); // Memory Addressing Mode
    OLED_WriteCommand(pOled, 0x10); // Page Addressing Mode
    OLED_WriteCommand(pOled, 0xB0); 
    OLED_WriteCommand(pOled, 0xC8); 
    OLED_WriteCommand(pOled, 0x00); 
    OLED_WriteCommand(pOled, 0x10); 
    OLED_WriteCommand(pOled, 0x40); 
    OLED_WriteCommand(pOled, 0x81); // Contrast
    OLED_WriteCommand(pOled, 0xFF);
    OLED_WriteCommand(pOled, 0xA1); // Segment Re-map
    OLED_WriteCommand(pOled, 0xA6); // Normal Display
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
    OLED_WriteCommand(pOled, 0x8D); // Charge Pump
    OLED_WriteCommand(pOled, 0x14); 
    OLED_WriteCommand(pOled, 0xAF); // Bật màn hình

    ECU_OLED_Clear();
    ECU_OLED_UpdateScreen(pOled);
}

void ECU_OLED_Clear(void) {
    for(int i = 0; i < 1024; i++) {
        OLED_Buffer[i] = 0x00;
    }
}

void ECU_OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if(x >= OLED_WIDTH || y >= OLED_HEIGHT) return;

    if(color == OLED_COLOR_WHITE) {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y % 8));
    } else {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}

void ECU_OLED_UpdateScreen(OLED_Handle_t *pOled) {
    uint8_t i2c_buff[129]; // 1 byte Control + 128 byte Data

    for(uint8_t page = 0; page < 8; page++) {
        OLED_WriteCommand(pOled, 0xB0 + page); 
        
        // Màn 1.3 inch (SH1106) bị lệch 2 cột -> Start cột tại 0x02
        OLED_WriteCommand(pOled, 0x02); // Low Column (Offset 2 pixel)
        OLED_WriteCommand(pOled, 0x10); // High Column

        // CHUẨN BỊ GÓI DATA
        i2c_buff[0] = 0x40; // Control Byte: DATA
        
        for(int i = 0; i < 128; i++) {
            i2c_buff[i+1] = OLED_Buffer[OLED_WIDTH * page + i];
        }

        // Gửi tẹt ga 1 gói 129 bytes
        MCAL_I2C_MasterSendData(pOled->pI2CHandle, i2c_buff, 129, OLED_I2C_ADDR);
    }
}


/* =========================================
   HÀM IN FONT CHUẨN 5x7
   ========================================= */
/* HÀM IN KÝ TỰ (Gọi từ bảng Font) */
void ECU_OLED_PrintChar_F5x7(uint8_t x, uint8_t y, char c) {
    // Trừ 32 vì bảng font bắt đầu từ Space (ASCII 32)
    if(c < 32 || c > 126) c = '?'; 
    
    for(uint8_t i = 0; i < OLED_FONT_WIDTH; i++) {
        // Lấy data từ file oled_font.c thông qua biến extern Font5x7
        uint8_t char_byte = Font5x7[c - 32][i]; 
        
        for(uint8_t j = 0; j < 8; j++) {
            if((char_byte >> j) & 0x01) {
                ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_WHITE);
            } else {
                ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_BLACK);
            }
        }
    }
}

/* HÀM IN CHUỖI */
void ECU_OLED_PrintString_F5x7(uint8_t x, uint8_t y, char *str) {
    while(*str) {
        if(x + OLED_FONT_WIDTH >= OLED_WIDTH) { // Hết dòng thì xuống dòng
            x = 0;
            y += 8;
        }
        ECU_OLED_PrintChar_F5x7(x, y, *str);
        x += (OLED_FONT_WIDTH + 1); // Dịch sang phải 6 pixel (5 font + 1 cách)
        str++;
    }
}



/* =========================================
   [MỚI] HÀM IN FONT NHỎ 3x5
   ========================================= */
void ECU_OLED_PrintChar_F3x5(uint8_t x, uint8_t y, char c) {
    if(c < 32 || c > 126) c = '?';
    
    // Vòng lặp chạy 3 lần (Width = 3)
    for(uint8_t i = 0; i < OLED_FONT_SMALL_WIDTH; i++) { 
        // Lấy dữ liệu từ mảng Font3x5
        uint8_t b = Font3x5[c - 32][i];
        
        for(uint8_t j = 0; j < 8; j++) {
            if((b >> j) & 0x01) ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_WHITE);
            else                ECU_OLED_DrawPixel(x + i, y + j, OLED_COLOR_BLACK);
        }
    }
}

void ECU_OLED_PrintString_F3x5(uint8_t x, uint8_t y, char *str) {
    while(*str) {
        // Xuống dòng nếu hết chỗ (Width 3 + 1 spacer = 4)
        if(x + 4 >= OLED_WIDTH) { 
            x = 0; y += 8;
        }
        ECU_OLED_PrintChar_F3x5(x, y, *str);
        x += (OLED_FONT_SMALL_WIDTH + 1); // Dịch 4 pixel
        str++;
    }
}



/* =========================================
   1. HÀM VẼ ĐƯỜNG THẲNG (Bresenham Line)
   ========================================= */
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

/* =========================================
   2. HÀM VẼ NỬA ĐƯỜNG TRÒN (Semi-Circle)
   ========================================= */
void ECU_OLED_DrawSemiCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
    int32_t f = 1 - r;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * r;
    int32_t x = 0;
    int32_t y = r;

    // Vẽ 2 điểm chốt ở ngang tâm (Trái và Phải)
    ECU_OLED_DrawPixel(x0 + r, y0, color);
    ECU_OLED_DrawPixel(x0 - r, y0, color);
    
    // Vẽ đỉnh trên cùng
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

        // Chỉ vẽ phần nằm phía trên tâm (y_plot < y0)
        // Upper Right
        ECU_OLED_DrawPixel(x0 + x, y0 - y, color);
        ECU_OLED_DrawPixel(x0 + y, y0 - x, color);
        
        // Upper Left
        ECU_OLED_DrawPixel(x0 - x, y0 - y, color);
        ECU_OLED_DrawPixel(x0 - y, y0 - x, color);
    }
}

/* =========================================
   3. HÀM VẼ ĐỒNG HỒ TỐC ĐỘ
   ========================================= */
#define PI 3.14159265

void ECU_OLED_DrawSpeedometer(uint8_t speed) {
    // Tọa độ tâm đồng hồ (Giữa màn hình, sát đáy)
    uint8_t cx = 64;
    uint8_t cy = 63; 
    uint8_t radius = 55; // Bán kính vòng cung

    // 1. Vẽ khung đồng hồ (Nửa vòng tròn)
    ECU_OLED_DrawSemiCircle(cx, cy, radius, OLED_COLOR_WHITE);
    // Vẽ thêm 1 vòng nhỏ hơn cho dày dặn (tùy chọn)
    ECU_OLED_DrawSemiCircle(cx, cy, radius - 1, OLED_COLOR_WHITE);

    // 2. Xử lý giới hạn tốc độ (0 - 100)
    if(speed > 100) speed = 100;

    // 3. Tính góc kim
    // Speed 0   -> Góc 180 độ (PI radian)   -> Bên trái
    // Speed 50  -> Góc 90 độ  (PI/2 radian) -> Thẳng đứng
    // Speed 100 -> Góc 0 độ   (0 radian)    -> Bên phải
    
    float angle = PI - ((float)speed / 100.0f) * PI;

    // 4. Tính tọa độ đầu kim
    // Kim ngắn hơn bán kính một chút (radius - 5)
    uint8_t needle_len = radius - 5;
    uint8_t tip_x = cx + (uint8_t)(cos(angle) * needle_len);
    uint8_t tip_y = cy - (uint8_t)(sin(angle) * needle_len); // Trừ vì trục Y OLED hướng xuống

    // 5. Vẽ kim
    ECU_OLED_DrawLine(cx, cy, tip_x, tip_y, OLED_COLOR_WHITE);

    // 6. Hiển thị số tốc độ ở giữa
    char buff[5];
    sprintf(buff, "%d", speed);
    
    // Căn giữa số: (Màn hình 128) - (Độ rộng chữ / 2)
    // Font 5x7 rộng 6px. Nếu số có 2 chữ số -> rộng 12px -> x = 64 - 6 = 58
    uint8_t text_x = 64 - (strlen(buff) * 6) / 2;
    ECU_OLED_PrintString_F5x7(text_x, cy - 10, buff); // Vẽ số ngay trên tâm
}
