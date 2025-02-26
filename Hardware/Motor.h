#ifndef MOTOR_H
#define MOTOR_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    GPIOxPiny_t PWM;
    GPIOxPiny_t IN1;
    GPIOxPiny_t IN2;
    uint32_t    IN1_ODR;
    uint32_t    IN2_ODR;

    TIM_TypeDef *TIMx;
    uint8_t      Channel;

    uint16_t        Range;
    FunctionalState Invert;
    FunctionalState TIM_Init;

    void (*TIM_SetCompare)(TIM_TypeDef *TIMx, uint16_t Compare1);
} Motor_t;

void Motor_Init(Motor_t *self);
void Motor_Set(Motor_t *self, int16_t speed);

#endif