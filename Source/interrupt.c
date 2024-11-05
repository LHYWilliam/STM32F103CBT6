#include "RTE_Components.h"
#include CMSIS_device_header

#include "Sampler.h"

extern Sampler_t Sampler;

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        Sampler.Index = (Sampler.Index + 1) % Sampler.Length;

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}