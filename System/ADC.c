#include <string.h>

#include "ADC.h"

void ADC_Init_(ADC_t *Self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx(Self->ADCx), ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    for (uint8_t channel = 1; channel <= Self->NbrOfChannel; channel++) {
        if ((Self->DMA == NULL && channel == 1) || (Self->DMA)) {
            ADC_RegularChannelConfig(Self->ADCx, Self->Channel[channel - 1],
                                     channel, ADC_SampleTime_55Cycles5);
        }
    }

    if (Self->TRGO) {
        ADC_ExternalTrigConvCmd(Self->ADCx, ENABLE);
    }

    ADC_InitTypeDef ADC_InitTStruct = {
        .ADC_NbrOfChannel = Self->NbrOfChannel,
        .ADC_ExternalTrigConv =
            Self->TRGO ? Self->TRGO : ADC_ExternalTrigConv_None,
        .ADC_ContinuousConvMode = Self->Continuous,
        .ADC_ScanConvMode =
            (Self->DMA && Self->NbrOfChannel > 1) ? ENABLE : DISABLE,
    };
    ADC_Init(Self->ADCx, &ADC_InitTStruct);

    if (Self->Cmd) {
        ADC_Cmd_(Self);
    }
}

void ADC_Cmd_(ADC_t *Self) {
    if (Self->DMA) {
        ADC_DMACmd(Self->ADCx, ENABLE);
    }

    ADC_Cmd(Self->ADCx, ENABLE);

    ADC_ResetCalibration(Self->ADCx);
    while (ADC_GetResetCalibrationStatus(Self->ADCx) == SET)
        ;
    ADC_StartCalibration(Self->ADCx);
    while (ADC_GetCalibrationStatus(Self->ADCx) == SET)
        ;

    if (Self->TRGO == NULL && Self->Continuous) {
        ADC_SoftwareStartConvCmd(Self->ADCx, ENABLE);
    }
}