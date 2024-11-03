#ifndef __PWM_H
#define __PWM_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char Channel[16];

    TIM_TypeDef *TIMx;
    uint16_t Prescaler;
    uint16_t Period;
    uint8_t TIM_Init;

    void (*TIM_SetCompare[4])(TIM_TypeDef *TIMx, uint16_t Compare);
} PWM_t;

void PWM_Init(PWM_t *self);

void PWM_SetPrescaler(PWM_t *self, uint16_t val);
void PWM_SetPulse(PWM_t *self, uint8_t channel, uint16_t pulse);

#endif