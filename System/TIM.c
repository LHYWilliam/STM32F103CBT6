#include <stdlib.h>

#include "Tim.h"

void TIM_Init(TIM_t *self, ClockSourceConfig_t *config) {
    RCC_APBxPeriphClockCmd(self->TIMx)(RCC_APBxPeriph_TIMx(self->TIMx), ENABLE);

    if (self->ClockSource) {
        self->ClockSource(self->TIMx, config);
    }

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
        .TIM_Prescaler = self->Prescaler,
        .TIM_Period = self->Period,
    };
    TIM_TimeBaseInit(self->TIMx, &TIM_TimeBaseInitStruct);

    if (self->TRGO) {
        TIM_SelectOutputTrigger(self->TIMx, self->TRGO);
    }

    if (self->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(self->TIMx, ENABLE);
    }

    if (self->Cmd) {
        TIM_ClearFlag(self->TIMx, TIM_FLAG_Update);
        TIM_Cmd(self->TIMx, ENABLE);
    }
}

void TIM_InternalClock(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_InternalClockConfig(TIMx);
}
void TIM_ETRClockMode2(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_ETRClockMode2Config(TIMx, config->TIM_ExtTRGPrescaler,
                            config->TIM_ExtTRGPolarity, config->ExtTRGFilter);
};