#include "main.h"

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        MQ2Sensor.Index = (MQ2Sensor.Index + 1) % MQ2Sensor.Length;
        MQ2Sensor.Data[MQ2Sensor.Index] = Sampler.Data[0];

        MQ3Sensor.Index = (MQ3Sensor.Index + 1) % MQ3Sensor.Length;
        MQ3Sensor.Data[MQ3Sensor.Index] = Sampler.Data[1];

        MQ7Sensor.Index = (MQ7Sensor.Index + 1) % MQ7Sensor.Length;
        MQ7Sensor.Data[MQ7Sensor.Index] = Sampler.Data[2];

        MQ135Sensor.Index = (MQ135Sensor.Index + 1) % MQ135Sensor.Length;
        MQ135Sensor.Data[MQ135Sensor.Index] = Sampler.Data[3];

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}