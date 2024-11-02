#ifndef __ENCODER_H
#define __ENCODER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char gpio[12];

    TIM_TypeDef *TIMx;

    uint8_t invert;
} Encoder_t;

void Encoder_Init(Encoder_t *self);

int16_t Encoder_Get(Encoder_t *self);

#endif