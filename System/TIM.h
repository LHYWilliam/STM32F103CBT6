#ifndef TIM_H
#define TIM_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdlib.h>

#define RCC_APBxPeriphClockCmd(x)                                              \
    ((x) == TIM1 ? RCC_APB2PeriphClockCmd : RCC_APB1PeriphClockCmd)

#define RCC_APBxPeriph_TIMx(x)                                                 \
    ((x) == TIM1   ? RCC_APB2Periph_TIM1                                       \
     : (x) == TIM2 ? RCC_APB1Periph_TIM2                                       \
     : (x) == TIM3 ? RCC_APB1Periph_TIM3                                       \
     : (x) == TIM4 ? RCC_APB1Periph_TIM4                                       \
                   : NULL)

#define TIMx_IRQn(x)                                                           \
    ((x) == TIM1   ? NULL                                                      \
     : (x) == TIM2 ? TIM2_IRQn                                                 \
     : (x) == TIM3 ? TIM3_IRQn                                                 \
     : (x) == TIM4 ? TIM4_IRQn                                                 \
                   : NULL)

static uint16_t TIM_Channel_x[] = {
    NULL, TIM_Channel_1, TIM_Channel_2, TIM_Channel_3, TIM_Channel_4,
};

typedef struct {
    uint16_t TIM_ExtTRGPrescaler;
    uint16_t TIM_ExtTRGPolarity;
    uint16_t ExtTRGFilter;
} ClockSourceConfig_t;

typedef struct {
    TIM_TypeDef *TIMx;
    void (*ClockSource)(TIM_TypeDef *TIMx, ClockSourceConfig_t *config);

    uint16_t Prescaler;
    uint16_t Period;

    uint8_t Cmd;

    uint16_t TRGO;
} TIM_t;

void TIM_Init(TIM_t *self, ClockSourceConfig_t *config);
void TIM_InternalClock(TIM_TypeDef *TIMx, ClockSourceConfig_t *config);
void TIM_ETRClockMode2(TIM_TypeDef *TIMx, ClockSourceConfig_t *config);

#endif