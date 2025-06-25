#include <stdlib.h>

#include "Tim.h"

void TIM_Init(TIM_t *Self, ClockSourceConfig_t *Config) {
    RCC_APBxPeriphClockCmd(Self->TIMx)(RCC_APBxPeriph_TIMx(Self->TIMx), ENABLE);

    if (Self->ClockSource) {
        Self->ClockSource(Self->TIMx, Config);
    }

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
        .TIM_Prescaler = Self->Prescaler,
        .TIM_Period    = Self->Period,
    };
    TIM_TimeBaseInit(Self->TIMx, &TIM_TimeBaseInitStruct);

    if (Self->TRGO) {
        TIM_SelectOutputTrigger(Self->TIMx, Self->TRGO);
    }

    if (Self->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(Self->TIMx, ENABLE);
    }

    if (Self->Cmd) {
        TIM_ClearFlag(Self->TIMx, TIM_FLAG_Update);
        TIM_Cmd(Self->TIMx, ENABLE);
    }
}

void TIM_InternalClock(TIM_TypeDef *TIMx, ClockSourceConfig_t *Config) {
    TIM_InternalClockConfig(TIMx);
}
void TIM_ETRClockMode2(TIM_TypeDef *TIMx, ClockSourceConfig_t *Config) {
    TIM_ETRClockMode2Config(TIMx, Config->TIM_ExtTRGPrescaler,
                            Config->TIM_ExtTRGPolarity, Config->ExtTRGFilter);
};