#include "Capture.h"

void Capture_Init(Capture_t *Self) {
    TIM_ICInitTypeDef TIM_ICInitStruct = {
        .TIM_Channel     = Self->TIM_Channel,
        .TIM_ICPolarity  = Self->TIM_ICPolarity,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,
        .TIM_ICFilter    = 0xF,
    };
    TIM_ICInit(Self->TIMx, &TIM_ICInitStruct);
}