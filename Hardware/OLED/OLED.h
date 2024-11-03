#ifndef __OLED_H
#define __OLED_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char SCL[4];
    char SDA[4];

    GPIO_TypeDef *SCL_GPIOx;
    uint16_t SCL_GPIO_Pin;
    GPIO_TypeDef *SDA_GPIOx;
    uint16_t SDA_GPIO_Pin;
} OLED_t;

void OLED_Init(OLED_t *self);
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

#endif