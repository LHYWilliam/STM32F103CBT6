#include <stdlib.h>

#include "Tim.h"

void TIM_Init(TIM_t *self, ClockSourceConfig_t *config) {
    if (self->TIMx == TIM1) {
        RCC_APB2PeriphClockCmd(RCC_APBxPeriph_TIMx(self->TIMx), ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(RCC_APBxPeriph_TIMx(self->TIMx), ENABLE);
    }

    if (self->ClockSource) {
        self->ClockSource(self->TIMx, config);
    }

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
        .TIM_Prescaler = self->Prescaler,
        .TIM_Period = self->Period,
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_ClockDivision = TIM_CKD_DIV1,
        .TIM_RepetitionCounter = 0,
    };
    TIM_TimeBaseInit(self->TIMx, &TIM_TimeBaseInitStruct);

    if (self->TIMx == TIM1) {
        TIM_CtrlPWMOutputs(self->TIMx, ENABLE);
    }

    if (self->CMD_Mode) {
        TIM_Cmd(self->TIMx, ENABLE);
        TIM_ClearFlag(self->TIMx, TIM_FLAG_Update);
    }
}

void TIM_InternalClock(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_InternalClockConfig(TIMx);
}
void TIM_ETRClockMode2(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_ETRClockMode2Config(TIMx, config->TIM_ExtTRGPrescaler,
                            config->TIM_ExtTRGPolarity, config->ExtTRGFilter);
};