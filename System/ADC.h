#ifndef ADC_H
#define ADC_H

#include "RTE_Components.h"
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

#define ADC_ExternalTrigConv_Tx_TRGO(x)                                        \
    ((x) == TIM3   ? ADC_ExternalTrigConv_T3_TRGO                              \
     : (x) == TIM8 ? ADC_ExternalTrigConv_T8_TRGO                              \
                   : NULL)

typedef struct {
    ADC_TypeDef *ADCx;
    char Channel[32];

    uint8_t Cmd;
    uint8_t DMA;
    uint8_t Continuous;

    uint32_t TRGO;

    uint16_t NbrOfChannel;
} ADC_t;

void ADC_Init_(ADC_t *self);
void ADC_Cmd_(ADC_t *self);

#endif