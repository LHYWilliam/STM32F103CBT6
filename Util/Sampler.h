#ifndef SAMPLER_H
#define SAMPLER_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    uint16_t *Data;
    uint16_t Index;
    uint16_t Length;

    GPIOxPiny_t GPIOxPiny[10];

    ADC_TypeDef *ADCx;
    uint8_t Continuous;
    uint8_t ADC_Channel[10];
    uint16_t NbrOfChannel;

    DMA_TypeDef *DMAx;
    uint8_t DMA_Channel;

    TIM_TypeDef *TIMx;
    uint16_t ms;
    uint16_t Hz;
    uint8_t Priority;
} Sampler_t;

void Sampler_Init(Sampler_t *self);
uint16_t Sampler_Get(Sampler_t *self, uint8_t Channel);

#endif