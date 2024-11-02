#ifndef __SERVO_H
#define __SERVO_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char PWM[32];

    TIM_TypeDef *TIMx;
    char channel[32];
    uint8_t TIM_Init;

    void (*TIM_SetCompare[2])(TIM_TypeDef *TIMx, uint16_t Compare);
} Servo_t;

void Servo_Init(Servo_t *self);

void Servo_set(Servo_t *self, float angle1, float angle2);

#endif