#ifndef LCD_H
#define LCD_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct LCD {
    GPIOxPiny_t SCL;
    GPIOxPiny_t SDA;
    GPIOxPiny_t RES;
    GPIOxPiny_t DC;
    GPIOxPiny_t CS;
    GPIOxPiny_t BL;
    uint32_t SCL_ODR;
    uint32_t SDA_ODR;
    uint32_t RES_ODR;
    uint32_t DC_ODR;
    uint32_t CS_ODR;
    uint32_t BL_ODR;

    uint8_t Width;
    uint8_t Height;

    uint8_t FontWidth;
    uint8_t FontHeight;

    uint8_t Direction;

    uint16_t PenColor;
    uint16_t BackColor;

    uint8_t PrintfBuffer[128];

    void (*WriteData8)(struct LCD *, uint8_t);
    void (*WriteData16)(struct LCD *, uint16_t);
    void (*WriteData16s)(struct LCD *, uint16_t, uint16_t);
    void (*WriteDatas16)(struct LCD *, uint16_t *, uint16_t);
    void (*WriteCommand)(struct LCD *, uint8_t);
} LCD_t;

void LCD_Init(LCD_t *self);
void LCD_SetPenColor(LCD_t *self, uint16_t Color);
void LCD_SetBackColor(LCD_t *self, uint16_t Color);

void LCD_Fill(LCD_t *self, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2,
              uint16_t Color);
void LCD_Clear(LCD_t *self);

void LCD_DrawPoint(LCD_t *self, uint16_t X, uint16_t Y);
void LCD_DrawHLine(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Length);
void LCD_DrawVLine(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Length);

void LCD_ShowChar(LCD_t *self, uint16_t X, uint16_t Y, uint8_t Char);
void LCD_ShowString(LCD_t *self, uint16_t X, uint16_t Y, char *String);
void LCD_Printf(LCD_t *self, int16_t X, int16_t Y, const char *Format, ...);

void LCD_ShowImage(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Width,
                   uint16_t Height, const uint8_t *Image);

#endif