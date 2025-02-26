#ifndef SERVO_H
#define SERVO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    GPIOxPiny_t PWM[4];

    TIM_TypeDef *TIMx;
    uint8_t      Channel[4];
    uint8_t      NbrOfChannel;

    FunctionalState TIM_Init;

    void (*TIM_SetCompare[4])(TIM_TypeDef *TIMx, uint16_t Compare);
} Servo_t;

void Servo_Init(Servo_t *self);
void Servo_SetAngle(Servo_t *self, uint8_t Channel, float Angle);

#endif