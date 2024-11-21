#include "main.h"

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        MQ3.Index = (MQ3.Index + 1) % MQ3.Length;
        MQ3.Data[MQ3.Index] = Sampler.Data[0];

        MQ135.Index = (MQ135.Index + 1) % MQ135.Length;
        MQ135.Data[MQ135.Index] = Sampler.Data[1];

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}