#include "Servo.h"
#include "GPIO.h"
#include "PWM.h"

void Servo_Init(Servo_t *self) {
    GPIO_t GPIO = {
        .Mode = GPIO_Mode_AF_PP,
    };
    GPIO_InitPin(GPIO, self->PWM);

    PWM_t PWM = {
        .TIMx = self->TIMx,
        .Prescaler = 72 - 1,
        .Period = 20000 - 1,
        .TIM_Init = self->TIM_Init,
    };
    strcpy(PWM.Channel, self->channel);
    PWM_Init(&PWM);

    self->TIM_SetCompare[0] = PWM.TIM_SetCompare[0];
    self->TIM_SetCompare[1] = PWM.TIM_SetCompare[1];
}

void Servo_set(Servo_t *self, float angle1, float angle2) {
    self->TIM_SetCompare[0](self->TIMx, angle1 / 180 * 2000 + 500);
    self->TIM_SetCompare[1](self->TIMx, angle2 / 180 * 2000 + 500);
}