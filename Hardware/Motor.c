#include "Motor.h"
#include "PWM.h"

void Motor_Init(Motor_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_Mode_AF_PP;
    GPIO_InitPin(&GPIO, Self->PWM);

    GPIO.Mode     = GPIO_Mode_Out_PP;
    Self->IN1_ODR = GPIO_InitPin(&GPIO, Self->IN1);
    Self->IN2_ODR = GPIO_InitPin(&GPIO, Self->IN2);

    uint8_t Channel[1] = {Self->Channel};

    PWM_t PWM = {
        .TIMx         = Self->TIMx,
        .Prescaler    = 100 - 1,
        .Period       = Self->Range - 1,
        .Channel      = Channel,
        .NbrOfChannel = 1,
        .TIM_Init     = Self->TIM_Init,
    };
    PWM_Init(&PWM);

    Self->TIM_SetCompare = PWM.TIM_SetCompare[0];
}
void Motor_Set(Motor_t *Self, int16_t Speed) {
    GPIO_Write(Self->IN1_ODR, Speed >= 0 ? Self->Invert : !Self->Invert);
    GPIO_Write(Self->IN2_ODR, Speed >= 0 ? !Self->Invert : Self->Invert);
    Self->TIM_SetCompare(Self->TIMx, (uint16_t)(Speed >= 0 ? Speed : -Speed));
}