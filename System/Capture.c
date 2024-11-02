#include "Capture.h"

void Capture_Init(Capture_t *capture) {
    TIM_ICInitTypeDef TIM_ICInitStruct = {
        capture->TIM_Channel, capture->TIM_ICPolarity, capture->TIM_ICSelection,
        TIM_ICPSC_DIV1,       capture->TIM_ICFilter,
    };
    TIM_ICInit(capture->TIMx, &TIM_ICInitStruct);
}