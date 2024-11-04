#include <string.h>

#include "ADC.h"

void ADC_Init_(ADC_t *self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx(self->ADCx), ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    self->NbrOfChannel = 1;
    char *temp = self->Channel;
    do {
        if ((self->DMA == NULL && self->NbrOfChannel == 1) || (self->DMA)) {
            ADC_RegularChannelConfig(self->ADCx, ADC_Channel_x(temp),
                                     self->NbrOfChannel,
                                     ADC_SampleTime_55Cycles5);
        }
    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2) &&
             (self->NbrOfChannel = self->NbrOfChannel + 1));

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