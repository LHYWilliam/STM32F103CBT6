#ifndef TIMER_H
#define TIMER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct Timer {
    TIM_TypeDef *TIMx;
    uint16_t ms;
    uint16_t Hz;

    uint8_t Interrupt;
    uint8_t Priority;

    uint16_t TRGO;
} Timer_t;

void Timer_Init(Timer_t *self);

#endif