#ifndef PWM_H
#define PWM_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    uint8_t *Channel;
    uint8_t  NbrOfChannel;

    TIM_TypeDef *TIMx;
    uint16_t     Prescaler;
    uint16_t     Period;
    uint8_t      TIM_Init;

    void (*TIM_SetCompare[4])(TIM_TypeDef *TIMx, uint16_t Compare);
} PWM_t;

void PWM_Init(PWM_t *Self);
void PWM_SetPrescaler(PWM_t *Self, uint16_t Prescaler);
void PWM_SetPulse(PWM_t *Self, uint8_t channel, uint16_t Pulse);

#endif