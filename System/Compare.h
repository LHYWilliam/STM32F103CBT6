#ifndef COMPARE_H
#define COMPARE_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define TIM_OCxInit(x)        \
    ((x) == 1   ? TIM_OC1Init \
     : (x) == 2 ? TIM_OC2Init \
     : (x) == 3 ? TIM_OC3Init \
     : (x) == 4 ? TIM_OC4Init \
                : NULL)

#define TIM_SetComparex(x)        \
    ((x) == 1   ? TIM_SetCompare1 \
     : (x) == 2 ? TIM_SetCompare2 \
     : (x) == 3 ? TIM_SetCompare3 \
     : (x) == 4 ? TIM_SetCompare4 \
                : NULL)

typedef struct {
    TIM_TypeDef *TIMx;
    uint16_t     Pulse;
    void (*TIM_OCInit)(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct);
    void (*TIM_SetCompare)(TIM_TypeDef *TIMx, uint16_t Compare1);
} Compare_t;

void Compare_Init(Compare_t *Self);

#endif