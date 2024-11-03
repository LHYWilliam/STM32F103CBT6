#include <string.h>

#include "GPIO.h"
#include "PWM.h"
#include "Servo.h"

void Servo_Init(Servo_t *self) {
    GPIO_t PWM_gpio = {
        .Mode = GPIO_Mode_AF_PP,
    };
    strcpy(PWM_gpio.GPIOxPiny, self->PWM);
    GPIO_Init_(&PWM_gpio);

    PWM_t pwm = {
        .TIMx = self->TIMx,
        .Prescaler = 72 - 1,
        .Period = 20000 - 1,
        .TIM_Init = self->TIM_Init,
    };
    strcpy(pwm.Channel, self->channel);
    PWM_Init(&pwm);

    self->TIM_SetCompare[0] = pwm.TIM_SetCompare[0];
    self->TIM_SetCompare[1] = pwm.TIM_SetCompare[1];
}

void Servo_set(Servo_t *self, float angle1, float angle2) {
    self->TIM_SetCompare[0](self->TIMx, angle1 / 180 * 2000 + 500);
    self->TIM_SetCompare[1](self->TIMx, angle2 / 180 * 2000 + 500);
}