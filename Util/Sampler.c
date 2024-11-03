#include <string.h>

#include "ADC.h"
#include "GPIO.h"
#include "Sampler.h"

void Sampler_Init_(Sampler_t *self) {
    ADC_t ADC = {
        .ADCx = self->ADCx,
        .Continuous = self->Continuous,
        .DMA = self->DMA,
    };
    strcpy(ADC.Channel, self->Channel);
    ADC_Init_(&ADC);

    GPIO_t GPIO = {
        .Mode = GPIO_Mode_AIN,
    };
    strcpy(GPIO.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init_(&GPIO);
}

uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel) {
    if (self->Continuous == DISABLE) {
        ADC_RegularChannelConfig(self->ADCx, ADC_Channel[Channel], 1,
                                 ADC_SampleTime_55Cycles5);

        ADC_SoftwareStartConvCmd(self->ADCx, ENABLE);
    }

    while (ADC_GetFlagStatus(self->ADCx, ADC_FLAG_EOC) == RESET)
        ;

    return ADC_GetConversionValue(self->ADCx);
}