#ifndef ADC_H
#define ADC_H

#include "RTE_Components.h"
#include <stdint.h>
#include CMSIS_device_header

#define RCC_APB2Periph_ADCx(x)                                                 \
    ((x) == ADC1   ? RCC_APB2Periph_ADC1                                       \
     : (x) == ADC2 ? RCC_APB2Periph_ADC2                                       \
                   : NULL)

#define ADC_Channel_x(x)                                                       \
    ((x[0]) == '0'   ? ADC_Channel_0                                           \
     : (x[0]) == '1' ? ADC_Channel_1                                           \
     : (x[0]) == '2' ? ADC_Channel_2                                           \
     : (x[0]) == '3' ? ADC_Channel_3                                           \
     : (x[0]) == '4' ? ADC_Channel_4                                           \
     : (x[0]) == '5' ? ADC_Channel_5                                           \
     : (x[0]) == '6' ? ADC_Channel_6                                           \
     : (x[0]) == '7' ? ADC_Channel_7                                           \
     : (x[0]) == '8' ? ADC_Channel_8                                           \
     : (x[0]) == '9' ? ADC_Channel_9                                           \
                     : NULL)

static uint8_t ADC_Channel[] = {
    ADC_Channel_0, ADC_Channel_1, ADC_Channel_2, ADC_Channel_3, ADC_Channel_4,
    ADC_Channel_5, ADC_Channel_6, ADC_Channel_7, ADC_Channel_8, ADC_Channel_9,
};

typedef struct {
    ADC_TypeDef *ADCx;
    char Channel[32];

    char GPIOxPiny[32];

    uint8_t Continuous;
    uint8_t DMA;
} ADC_t;

void ADC_Init_(ADC_t *self);

uint16_t ADC_Get(ADC_t *self, uint8_t Channel);

#endif