#include "Motor.h"
#include "PWM.h"

void Motor_Init(Motor_t *self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_Mode_AF_PP;
    GPIO_InitPin(&GPIO, self->PWM);

    GPIO.Mode = GPIO_Mode_Out_PP;
    GPIO_InitPin(&GPIO, self->IN1);
    self->IN1_GPIOx = GPIO.GPIOx;
    self->IN1_GPIO_Pin = GPIO.GPIO_Pin;
    GPIO_InitPin(&GPIO, self->IN2);
    self->IN2_GPIOx = GPIO.GPIOx;
    self->IN2_GPIO_Pin = GPIO.GPIO_Pin;

    uint8_t Channel[1] = {self->Channel};
    PWM_t PWM = {
        .TIMx = self->TIMx,
        .Prescaler = 100 - 1,
        .Period = 7200 - 1,
        .Channel = Channel,
        .NbrOfChannel = 1,
        .TIM_Init = self->TIM_Init,
    };
    PWM_Init(&PWM);

    self->TIM_SetCompare = PWM.TIM_SetCompare[0];
}
void Motor_Set(Motor_t *self, int16_t speed) {
    GPIO_WriteBit(self->IN1_GPIOx, self->IN1_GPIO_Pin,
                  (BitAction)(speed >= 0 ? self->Invert : !self->Invert));
    GPIO_WriteBit(self->IN2_GPIOx, self->IN2_GPIO_Pin,
                  (BitAction)(speed >= 0 ? !self->Invert : self->Invert));
    self->TIM_SetCompare(self->TIMx, (uint16_t)(speed >= 0 ? speed : -speed));
}