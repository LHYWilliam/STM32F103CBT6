#include "main.h"

// void TIM3_IRQHandler() {
//     if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {

//         for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++)
//         {
//             MQSensor[i].Index = (MQSensor[i].Index + 1) % MQSensor[i].Length;
//             MQSensor[i].Data[MQSensor[i].Index] = Sampler.Data[i];
//         }

//         TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
//     }
// }