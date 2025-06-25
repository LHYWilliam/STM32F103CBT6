#include "Sampler.h"
#include "ADC.h"
#include "DMA.h"
#include "Timer.h"

void Sampler_Init(Sampler_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_AIN;

    for (uint8_t i = 0; i < 10; i++) {
        if (*Self->GPIOxPiny[i]) {
            GPIO_InitPin(&GPIO, Self->GPIOxPiny[i]);
            Self->NbrOfChannel++;
        }
    }

    ADC_t ADC = {
        .ADCx         = Self->ADCx,
        .Cmd          = DISABLE,
        .Channel      = Self->ADC_Channel,
        .NbrOfChannel = Self->NbrOfChannel,
        .Continuous   = Self->Continuous,
        .DMA          = Self->DMAx ? ENABLE : DISABLE,
        .TRGO         = ADC_ExternalTrigConv_Tx_TRGO(Self->TIMx),
    };
    ADC_Init_(&ADC);

    if (Self->DMAx) {
        DMA_t DMA = {
            .DMAx       = Self->DMAx,
            .channel    = Self->DMA_Channel,
            .sourceAddr = (uint32_t)&(Self->ADCx->DR),
            .sourceInc  = DISABLE,
            .targetAddr = (uint32_t)Self->Data,
            .targetInc  = ENABLE,
            .DataSize   = 16,
            .BufferSize = Self->Length,
            .Circular   = ENABLE,
        };

        DMA_Init_(&DMA);
        DMA_Cmd_(&DMA);
    }

    ADC_Cmd_(&ADC);

    if (Self->TIMx) {
        Timer_t Timer = {
            .TIMx      = Self->TIMx,
            .Hz        = Self->Hz,
            .ms        = Self->ms,
            .Interrupt = ENABLE,
            .Priority  = Self->Priority,
            .TRGO      = TIM_TRGOSource_Update,
        };
        Timer_Init(&Timer);
    }

    Self->Index = Self->Length - 1;
}

uint16_t Sampler_Get(Sampler_t *Self, uint8_t Channel) {
    static uint8_t ADC_Channel[] = {
        ADC_Channel_0, ADC_Channel_1, ADC_Channel_2, ADC_Channel_3,
        ADC_Channel_4, ADC_Channel_5, ADC_Channel_6, ADC_Channel_7,
        ADC_Channel_8, ADC_Channel_9,
    };

    if (Self->NbrOfChannel > 1) {
        ADC_RegularChannelConfig(Self->ADCx, ADC_Channel[Channel], 1,
                                 ADC_SampleTime_55Cycles5);
    }

    if (Self->Continuous == NULL) {
        ADC_SoftwareStartConvCmd(Self->ADCx, ENABLE);
    }

    while (ADC_GetFlagStatus(Self->ADCx, ADC_FLAG_EOC) == RESET)
        ;

    if (Self->Data) {
        Self->Index             = (Self->Index + 1) % Self->Length;
        Self->Data[Self->Index] = ADC_GetConversionValue(Self->ADCx);

        return Self->Data[Self->Index];
    } else {
        return ADC_GetConversionValue(Self->ADCx);
    }
}