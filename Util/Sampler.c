#include "Sampler.h"
#include "ADC.h"
#include "DMA.h"
#include "Timer.h"

void Sampler_Init(Sampler_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_AIN;

    for (uint8_t i = 0; i < 10; i++) {
        if (*self->GPIOxPiny[i]) {
            GPIO_InitPin(&GPIO, self->GPIOxPiny[i]);
            self->NbrOfChannel++;
        }
    }

    ADC_t ADC = {
        .ADCx         = self->ADCx,
        .Cmd          = DISABLE,
        .Channel      = self->ADC_Channel,
        .NbrOfChannel = self->NbrOfChannel,
        .Continuous   = self->Continuous,
        .DMA          = self->DMAx ? ENABLE : DISABLE,
        .TRGO         = ADC_ExternalTrigConv_Tx_TRGO(self->TIMx),
    };
    ADC_Init_(&ADC);

    if (self->DMAx) {
        DMA_t DMA = {
            .DMAx       = self->DMAx,
            .channel    = self->DMA_Channel,
            .sourceAddr = (uint32_t)&(self->ADCx->DR),
            .sourceInc  = DISABLE,
            .targetAddr = (uint32_t)self->Data,
            .targetInc  = ENABLE,
            .DataSize   = 16,
            .BufferSize = self->Length,
            .Circular   = ENABLE,
        };

        DMA_Init_(&DMA);
        DMA_Cmd_(&DMA);
    }

    ADC_Cmd_(&ADC);

    if (self->TIMx) {
        Timer_t Timer = {
            .TIMx      = self->TIMx,
            .Hz        = self->Hz,
            .ms        = self->ms,
            .Interrupt = ENABLE,
            .Priority  = self->Priority,
            .TRGO      = TIM_TRGOSource_Update,
        };
        Timer_Init(&Timer);
    }

    self->Index = self->Length - 1;
}

uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel) {
    static uint8_t ADC_Channel[] = {
        ADC_Channel_0, ADC_Channel_1, ADC_Channel_2, ADC_Channel_3,
        ADC_Channel_4, ADC_Channel_5, ADC_Channel_6, ADC_Channel_7,
        ADC_Channel_8, ADC_Channel_9,
    };

    if (self->NbrOfChannel > 1) {
        ADC_RegularChannelConfig(self->ADCx, ADC_Channel[Channel], 1,
                                 ADC_SampleTime_55Cycles5);
    }

    if (self->Continuous == NULL) {
        ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
    }

    while (ADC_GetFlagStatus(self->ADCx, ADC_FLAG_EOC) == RESET)
        ;

    if (self->Data) {
        self->Index             = (self->Index + 1) % self->Length;
        self->Data[self->Index] = ADC_GetConversionValue(self->ADCx);

        return self->Data[self->Index];
    } else {
        return ADC_GetConversionValue(self->ADCx);
    }
}