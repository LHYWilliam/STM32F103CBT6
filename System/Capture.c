#include "Capture.h"

void Capture_Init(Capture_t *capture) {
    TIM_ICInitTypeDef TIM_ICInitStruct = {
        .TIM_Channel = capture->TIM_Channel,
        .TIM_ICPolarity = capture->TIM_ICPolarity,
        .TIM_ICSelection = capture->TIM_ICSelection,
        .TIM_ICFilter = capture->TIM_ICFilter,
    };
    TIM_ICInit(capture->TIMx, &TIM_ICInitStruct);
}