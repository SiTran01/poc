#ifndef OLED_FONT_H
#define OLED_FONT_H

#include <stdint.h>

/* --- FONT 5x7 (M?c d?nh) --- */
#define OLED_FONT_WIDTH  5
extern const uint8_t Font5x7[][5];

/* --- FONT 3x5 (Siêu nh?) --- */
#define OLED_FONT_SMALL_WIDTH  3
extern const uint8_t Font3x5[][3];

#endif