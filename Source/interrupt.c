#include "RTE_Components.h"
#include CMSIS_device_header

#include "Sampler.h"

#define MQx_DataLength 128

extern Sampler_t Sampler;

extern int16_t MQ3_Index;
extern uint16_t MQ3_Data[128];

extern int16_t MQ135_Index;
extern uint16_t MQ135_Data[128];

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        MQ3_Index = (MQ3_Index + 1) % MQx_DataLength;
        MQ3_Data[MQ3_Index] = Sampler.Data[0];

        MQ135_Index = (MQ135_Index + 1) % MQx_DataLength;
        MQ135_Data[MQ135_Index] = Sampler.Data[1];

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}