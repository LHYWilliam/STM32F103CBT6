#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    uint32_t Font;
    uint8_t  FontWidth;
    uint8_t  FontHeight;
} LCDFont_t;

extern LCDFont_t LCDFonts[3];

extern const uint8_t LCD_Font8x9[][9];
extern const uint8_t LCD_Font8x16[][16];
extern const uint8_t LCD_Font12x16[][32];