#ifndef CAPTURE_H
#define CAPTURE_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    TIM_TypeDef *TIMx;
    uint16_t     TIM_Channel;
    uint16_t     TIM_ICPolarity;
    uint16_t (*TIM_GetCapture)(TIM_TypeDef *TIMx);
} Capture_t;

void Capture_Init(Capture_t *Self);

#endif