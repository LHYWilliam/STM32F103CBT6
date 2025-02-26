#include "Compare.h"

void Compare_Init(Compare_t *self) {
    TIM_OCInitTypeDef TIM_OCInitStruct = {
        .TIM_OCMode      = TIM_OCMode_PWM1,
        .TIM_OCPolarity  = TIM_OCPolarity_High,
        .TIM_OutputState = TIM_OutputState_Enable,
        .TIM_Pulse       = self->Pulse,
    };
    self->TIM_OCInit(self->TIMx, &TIM_OCInitStruct);
}