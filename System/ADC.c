#include <string.h>

#include "ADC.h"

void ADC_Init_(ADC_t *self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx(self->ADCx), ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    uint8_t NbrOfChannel = 1;
    char *temp = self->Channel;
    do {
        if (self->Continuous == ENABLE) {
            ADC_RegularChannelConfig(self->ADCx, ADC_Channel_x(temp),
                                     NbrOfChannel, ADC_SampleTime_55Cycles5);
        }
    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2) &&
             (NbrOfChannel = NbrOfChannel + 1));

    ADC_InitTypeDef ADC_InitTStruct = {
        .ADC_NbrOfChannel = NbrOfChannel,
        .ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
        .ADC_ContinuousConvMode = self->Continuous,
        .ADC_ScanConvMode =
            (NbrOfChannel > 1 && self->Continuous) ? ENABLE : DISABLE,
    };
    ADC_Init(self->ADCx, &ADC_InitTStruct);

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

    if (self->Continuous == ENABLE) {
        ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
    }
}

uint16_t ADC_Get(ADC_t *self, uint8_t Channel) {
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    return ADC_GetConversionValue(self->ADCx);
}