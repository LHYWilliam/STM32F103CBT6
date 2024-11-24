#include "main.h"

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        MQ2.Index = (MQ2.Index + 1) % MQ2.Length;
        MQ2.Data[MQ2.Index] = Sampler.Data[0];

        MQ3.Index = (MQ3.Index + 1) % MQ3.Length;
        MQ3.Data[MQ3.Index] = Sampler.Data[1];

        MQ7.Index = (MQ7.Index + 1) % MQ7.Length;
        MQ7.Data[MQ7.Index] = Sampler.Data[2];

        MQ135.Index = (MQ135.Index + 1) % MQ135.Length;
        MQ135.Data[MQ135.Index] = Sampler.Data[3];

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}