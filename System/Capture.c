#include "Capture.h"

void Capture_Init(Capture_t *self) {
    TIM_ICInitTypeDef TIM_ICInitStruct = {
        .TIM_Channel     = self->TIM_Channel,
        .TIM_ICPolarity  = self->TIM_ICPolarity,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,
        .TIM_ICFilter    = 0xF,
    };
    TIM_ICInit(self->TIMx, &TIM_ICInitStruct);
}