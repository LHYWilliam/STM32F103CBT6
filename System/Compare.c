#include "Compare.h"

void Compare_Init(Compare_t *Self) {
    TIM_OCInitTypeDef TIM_OCInitStruct = {
        .TIM_OCMode      = TIM_OCMode_PWM1,
        .TIM_OCPolarity  = TIM_OCPolarity_High,
        .TIM_OutputState = TIM_OutputState_Enable,
        .TIM_Pulse       = Self->Pulse,
    };
    Self->TIM_OCInit(Self->TIMx, &TIM_OCInitStruct);
}