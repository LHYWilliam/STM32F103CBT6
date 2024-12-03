#ifndef ENCODER_H
#define ENCODER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char GPIOxPiny[12];

    TIM_TypeDef *TIMx;

    uint8_t Invert;
} Encoder_t;

void Encoder_Init(Encoder_t *self);
int16_t Encoder_GetCount(Encoder_t *self);

#endif