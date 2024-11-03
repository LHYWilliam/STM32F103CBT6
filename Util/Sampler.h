#ifndef SAMPLER_H
#define SAMPLER_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    ADC_TypeDef *ADCx;
    char Channel[32];

    char GPIOxPiny[32];

    uint8_t Continuous;
    uint8_t DMA;
} Sampler_t;

void Sampler_Init_(Sampler_t *self);

uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel);

#endif