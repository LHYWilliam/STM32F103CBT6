#include "Servo.h"
#include "PWM.h"

void Servo_Init(Servo_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_AF_PP;

    for (uint8_t i = 0; i < 10; i++) {
        if (*Self->PWM[i]) {
            GPIO_InitPin(&GPIO, Self->PWM[i]);
            Self->NbrOfChannel++;
        }
    }

    PWM_t PWM = {
        .TIMx         = Self->TIMx,
        .Prescaler    = 72 - 1,
        .Period       = 20000 - 1,
        .Channel      = Self->Channel,
        .NbrOfChannel = Self->NbrOfChannel,
        .TIM_Init     = Self->TIM_Init,
    };
    PWM_Init(&PWM);

    for (uint8_t i = 0; i < Self->NbrOfChannel; i++) {
        Self->TIM_SetCompare[i] = PWM.TIM_SetCompare[i];
    }
}

void Servo_SetAngle(Servo_t *Self, uint8_t Channel, float Angle) {
    Self->TIM_SetCompare[Channel - 1](Self->TIMx, Angle / 180 * 2000 + 500);
}