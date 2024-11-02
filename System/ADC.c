#include <string.h>

#include "ADC.h"
#include "GPIO.h"

void ADC_Init_(ADC_t *self) {
    GPIO_t gpio = {
        .Mode = GPIO_Mode_AIN,
    };
    strcpy(gpio.GPIOxPiny, self->gpio);
    GPIO_Init_(&gpio);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx(self->ADCx), ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    uint8_t NbrOfChannel = 1;
    char *temp = self->channel;
    do {
        ADC_RegularChannelConfig(self->ADCx, ADC_Channel_x(temp), NbrOfChannel,
                                 ADC_SampleTime_55Cycles5);
    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2) &&
             (NbrOfChannel = NbrOfChannel + 1));

    ADC_InitTypeDef ADC_InitTStruct = {
        .ADC_Mode = ADC_Mode_Independent,
        .ADC_NbrOfChannel = NbrOfChannel,
        .ADC_DataAlign = ADC_DataAlign_Right,
        .ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
        .ADC_ContinuousConvMode = ENABLE,
        .ADC_ScanConvMode = NbrOfChannel > 1 ? ENABLE : DISABLE,
    };
    ADC_Init(self->ADCx, &ADC_InitTStruct);
}

void ADC_Start(ADC_t *self) {
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

    ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
}

uint16_t ADC_Get(ADC_t *self) {
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;

    return ADC_GetConversionValue(self->ADCx);
}