#include <string.h>

#include "ADC.h"

void ADC_Init_(ADC_t *self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx(self->ADCx), ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    for (uint8_t channel = 1; channel <= self->NbrOfChannel; channel++) {
        if ((self->DMA == NULL && channel == 1) || (self->DMA)) {
            ADC_RegularChannelConfig(self->ADCx, self->Channel[channel - 1],
                                     channel, ADC_SampleTime_55Cycles5);
        }
    }

    if (self->TRGO) {
        ADC_ExternalTrigConvCmd(self->ADCx, ENABLE);
    }

    ADC_InitTypeDef ADC_InitTStruct = {
        .ADC_NbrOfChannel = self->NbrOfChannel,
        .ADC_ExternalTrigConv =
            self->TRGO ? self->TRGO : ADC_ExternalTrigConv_None,
        .ADC_ContinuousConvMode = self->Continuous,
        .ADC_ScanConvMode =
            (self->DMA && self->NbrOfChannel > 1) ? ENABLE : DISABLE,
    };
    ADC_Init(self->ADCx, &ADC_InitTStruct);

    if (self->Cmd) {
        ADC_Cmd_(self);
    }
}

void ADC_Cmd_(ADC_t *self) {
    if (self->DMA) {
        ADC_DMACmd(self->ADCx, ENABLE);
    }

    ADC_Cmd(self->ADCx, ENABLE);

    ADC_ResetCalibration(self->ADCx);
    while (ADC_GetResetCalibrationStatus(self->ADCx) == SET)
        ;
    ADC_StartCalibration(self->ADCx);
    while (ADC_GetCalibrationStatus(self->ADCx) == SET)
        ;

    if (self->TRGO == NULL && self->Continuous) {
        ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
    }
}