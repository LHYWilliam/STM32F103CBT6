#ifndef TIMER_H
#define TIMER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct Timer {
    TIM_TypeDef *TIMx;
    uint16_t ms;
} Timer_t;

void Timer_Init(Timer_t *self);

#endif