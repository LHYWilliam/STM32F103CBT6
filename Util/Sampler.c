#include <string.h>

#include "ADC.h"
#include "DMA.h"
#include "GPIO.h"
#include "Sampler.h"
#include "Timer.h"

void Sampler_Init(Sampler_t *self) {
    GPIO_t GPIO = {
        .Mode = GPIO_Mode_AIN,
    };
    strcpy(GPIO.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init_(&GPIO);

    ADC_t ADC = {
        .ADCx = self->ADCx,
        .Cmd = DISABLE,
        .Continuous = self->Continuous,
        .DMA = self->DMAx ? ENABLE : DISABLE,
        .TRGO = ADC_ExternalTrigConv_Tx_TRGO(self->TIMx),
    };
    strcpy(ADC.Channel, self->ADC_Channel);
    ADC_Init_(&ADC);

    if (self->DMAx) {
        DMA_t DMA = {
            .DMAx = self->DMAx,
            .channel = self->DMA_Channel,
            .sourceAddr = (uint32_t) & (self->ADCx->DR),
            .sourceInc = DISABLE,
            .targetAddr = (uint32_t)self->Data,
            .targetInc = ENABLE,
            .DataSize = 16,
            .BufferSize = self->Length,
            .Circular = ENABLE,
            .M2M = DISABLE,
        };

        DMA_Init_(&DMA);
        DMA_Cmd_(&DMA);
    }

    ADC_Cmd_(&ADC);

    Timer_t Timer = {
        .TIMx = self->TIMx,
        .Hz = self->Hz,
        .ms = self->ms,
        .TRGO = TIM_TRGOSource_Update,
    };
    Timer_Init(&Timer);
}

uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel) {
    if (self->Continuous == DISABLE && self->DMAx == NULL) {
        ADC_RegularChannelConfig(self->ADCx, ADC_Channel[Channel], 1,
                                 ADC_SampleTime_55Cycles5);

        ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
    }

    while (ADC_GetFlagStatus(self->ADCx, ADC_FLAG_EOC) == RESET)
        ;

    return ADC_GetConversionValue(self->ADCx);
}