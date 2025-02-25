#ifndef OLED_H
#define OLED_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"
#include "OLED_Font.h"

#define OLED_IndexToX(Index, Length, X, Width)                                 \
    ((Index) * (Width - 1) / (Length - 1) + X)
#define OLED_ADCToY(ADC, Y, Height) ((4095 - ADC) * (Height - 1) / 4095 + Y)

#define U8G2                        0

#if U8G2

#include "u8g2.h"

#endif

typedef struct OLED_t {
    uint8_t I2C;
    I2C_TypeDef *I2Cx;
    GPIOxPiny_t SCL;
    GPIOxPiny_t SDA;
    uint32_t SCL_ODR;
    uint32_t SDA_ODR;

    uint8_t SPI;
    SPI_TypeDef *SPIx;
    GPIOxPiny_t D0;
    GPIOxPiny_t D1;
    GPIOxPiny_t RES;
    GPIOxPiny_t DC;
    GPIOxPiny_t CS;
    uint32_t D0_ODR;
    uint32_t D1_ODR;
    uint32_t RES_ODR;
    uint32_t DC_ODR;
    uint32_t CS_ODR;

    uint8_t Width;
    uint8_t Height;

    void (*OLED_WriteDatas)(struct OLED_t *self, uint8_t *Datas,
                            uint16_t Length);
    void (*OLED_WriteCommand)(struct OLED_t *self, uint8_t Command);
    void (*OLED_WriteCommands)(struct OLED_t *self, uint8_t *Commands,
                               uint16_t Length);

    uint8_t PrintfBuffer[128];
    uint8_t CursorCommandsBuffer[3];
    uint8_t DisplayBuffer[8][128];

    OLEDFont Font;
    uint8_t FontWidth;
    uint8_t FontHeight;

#if U8G2

    uint8_t U8g2;
    u8g2_t u8g2;

#endif

} OLED_t;

void OLED_Init(OLED_t *self);

#if U8G2

void u8g2_Printf(OLED_t *self, u8g2_uint_t x, u8g2_uint_t y, const char *format,
                 ...);

#endif

void OLED_Fill(OLED_t *self);
void OLED_FillArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                   uint8_t Height);
void OLED_Clear(OLED_t *self);
void OLED_ClearArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height);
void OLED_Reverse(OLED_t *self);
void OLED_ReverseArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                      uint8_t Height);

void OLED_DrawPoint(OLED_t *self, int16_t X, int16_t Y);
void OLED_DrawHLine(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t step);
void OLED_DrawVLine(OLED_t *self, int16_t X, int16_t Y, uint8_t Height,
                    uint8_t step);
void OLED_DrawLine(OLED_t *self, int16_t X0, int16_t Y0, int16_t X1,
                   int16_t Y1);
void OLED_DrawHollowRectangle(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                              uint8_t Height);
void OLED_ShowChart(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, uint16_t *Data, uint16_t Length,
                    int16_t Index);

void OLED_ShowImage(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, const uint8_t *Image);

void OLED_SetFont(OLED_t *self, OLEDFont Font);
void OLED_ShowChar(OLED_t *self, int16_t X, int16_t Y, char Char);
void OLED_ShowString(OLED_t *self, int16_t X, int16_t Y, const char *String);
void OLED_Printf(OLED_t *self, int16_t x, int16_t y, const char *format, ...);

void OLED_ClearBuffer(OLED_t *self);
void OLED_ClearBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                          uint8_t Height);
void OLED_SendBuffer(OLED_t *self);
void OLED_SendBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                         uint8_t Height);

#endif