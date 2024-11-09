#ifndef OLED_H
#define OLED_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define U8G2 0

#if U8G2

#include "u8g2.h"

#endif

typedef struct {
    uint8_t I2C;
    char SCL[4];
    char SDA[4];

    uint32_t SCL_ODR;
    uint32_t SDA_ODR;

    uint8_t Buffer[128];

#if U8G2
    uint8_t Width;
    uint8_t Height;

    uint8_t SPI;
    SPI_TypeDef *SPIx;
    char D0[4];
    char D1[4];
    char RES[4];
    char DC[4];
    char CS[4];

    uint8_t U8g2;
    u8g2_t u8g2;

#endif

} OLED_t;

void OLED_Init(OLED_t *self);

#if U8G2

void u8g2_Printf(OLED_t *self, u8g2_uint_t x, u8g2_uint_t y, const char *format,
                 ...);

#else

void OLED_Clear(OLED_t *self);

void OLED_ShowChar(OLED_t *self, uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(OLED_t *self, uint8_t Line, uint8_t Column, char *String);

void OLED_ShowNum(OLED_t *self, uint8_t Line, uint8_t Column, uint32_t Number,
                  uint8_t Length);
void OLED_ShowSignedNum(OLED_t *self, uint8_t Line, uint8_t Column,
                        int32_t Number, uint8_t Length);
void OLED_ShowHexNum(OLED_t *self, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(OLED_t *self, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length);

void OLED_Printf(OLED_t *self, uint16_t x, uint16_t y, const char *format, ...);

#endif

#endif