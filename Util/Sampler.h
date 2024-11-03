#ifndef SAMPLER_H
#define SAMPLER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    uint16_t *Data;
    uint16_t Length;

    ADC_TypeDef *ADCx;
    char ADC_Channel[32];

    char GPIOxPiny[32];

    DMA_TypeDef *DMAx;
    uint8_t DMA_Channel;

    uint8_t Continuous;
} Sampler_t;

void Sampler_Init_(Sampler_t *self);

uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel);

#endif