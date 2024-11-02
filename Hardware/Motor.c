#include <string.h>

#include "GPIO.h"
#include "Motor.h"
#include "PWM.h"

void Motor_Init(Motor_t *self) {
    GPIO_t PWM_gpio = {
        .Mode = GPIO_Mode_AF_PP,
    };
    strcpy(PWM_gpio.GPIOxPiny, self->PWM);
    GPIO_Init_(&PWM_gpio);

    GPIO_t IN1 = {
        .Mode = GPIO_Mode_Out_PP,
    };
    strcpy(IN1.GPIOxPiny, self->IN1);
    GPIO_Init_(&IN1);

    GPIO_t IN2 = {
        .Mode = GPIO_Mode_Out_PP,
    };
    strcpy(IN2.GPIOxPiny, self->IN2);
    GPIO_Init_(&IN2);

    self->IN1_GPIOx = IN1.GPIOx;
    self->IN1_GPIO_Pin = IN1.GPIO_Pin;
    self->IN2_GPIOx = IN2.GPIOx;
    self->IN2_GPIO_Pin = IN2.GPIO_Pin;

    PWM_t pwm = {
        .TIMx = self->TIMx,
        .Prescaler = 100 - 1,
        .Period = 7200 - 1,
        .TIM_Init = self->TIM_Init,
    };
    strcpy(pwm.channel, self->channel);
    PWM_Init(&pwm);

    self->TIM_SetCompare = pwm.TIM_SetCompare[0];
    self->set_Mode = self->invert ? Bit_SET : Bit_RESET;
}
void Motor_Set(Motor_t *self, int16_t speed) {
    GPIO_WriteBit(self->IN1_GPIOx, self->IN1_GPIO_Pin,
                  speed >= 0 ? self->set_Mode : !self->set_Mode);
    GPIO_WriteBit(self->IN2_GPIOx, self->IN2_GPIO_Pin,
                  speed >= 0 ? !self->set_Mode : self->set_Mode);
    self->TIM_SetCompare(self->TIMx, (uint16_t)(speed >= 0 ? speed : -speed));
}