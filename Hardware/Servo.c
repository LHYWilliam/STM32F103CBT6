#include "Servo.h"
#include "PWM.h"

void Servo_Init(Servo_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_AF_PP;

    for (uint8_t i = 0; i < 10; i++) {
        if (*self->PWM[i]) {
            GPIO_InitPin(&GPIO, self->PWM[i]);
            self->NbrOfChannel++;
        }
    }

    PWM_t PWM = {
        .TIMx = self->TIMx,
        .Prescaler = 72 - 1,
        .Period = 20000 - 1,
        .Channel = self->Channel,
        .NbrOfChannel = self->NbrOfChannel,
        .TIM_Init = self->TIM_Init,
    };
    PWM_Init(&PWM);

    for (uint8_t i = 0; i < self->NbrOfChannel; i++) {
        self->TIM_SetCompare[i] = PWM.TIM_SetCompare[i];
    }
}

void Servo_SetAngle(Servo_t *self, uint8_t Channel, float Angle) {
    self->TIM_SetCompare[Channel - 1](self->TIMx, Angle / 180 * 2000 + 500);
}