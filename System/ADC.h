#ifndef ADC_H
#define ADC_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define RCC_APB2Periph_ADCx(x)                                                 \
    ((x) == ADC1   ? RCC_APB2Periph_ADC1                                       \
     : (x) == ADC2 ? RCC_APB2Periph_ADC2                                       \
                   : NULL)

#define ADC_ExternalTrigConv_Tx_TRGO(x)                                        \
    ((x) == TIM3   ? ADC_ExternalTrigConv_T3_TRGO                              \
     : (x) == TIM8 ? ADC_ExternalTrigConv_T8_TRGO                              \
                   : NULL)

typedef struct {
    ADC_TypeDef *ADCx;
    uint8_t *Channel;
    uint16_t NbrOfChannel;

    FunctionalState Cmd;
    FunctionalState DMA;
    FunctionalState Continuous;

    uint32_t TRGO;
} ADC_t;

void ADC_Init_(ADC_t *self);
void ADC_Cmd_(ADC_t *self);

#endif