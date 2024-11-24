#ifndef MOTOR_H
#define MOTOR_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    char PWM[4];
    char IN1[4];
    char IN2[4];

    TIM_TypeDef *TIMx;
    uint8_t Channel;

    uint8_t Invert;
    uint8_t TIM_Init;

    GPIO_TypeDef *IN1_GPIOx;
    uint16_t IN1_GPIO_Pin;
    GPIO_TypeDef *IN2_GPIOx;
    uint16_t IN2_GPIO_Pin;

    void (*TIM_SetCompare)(TIM_TypeDef *TIMx, uint16_t Compare1);
} Motor_t;

void Motor_Init(Motor_t *self);

void Motor_Set(Motor_t *self, int16_t speed);

#endif