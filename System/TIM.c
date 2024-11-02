#include <stdlib.h>

#include "Interrupt.h"
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

void Timer_Init(Timer_t *self) {
    TIM_t tim = {
        .TIMx = self->TIMx,
        .ClockSource = TIM_InternalClock,
        .Prescaler = 7200 - 1,
        .Period = self->ms * 10 - 1,
        .CMD_Mode = UNCMD,
    };
    TIM_Init(&tim, NULL);

    TIMInterrupt_t TIM_interrupt = {
        .TIMx = self->TIMx,
        .NVIC_IRQChannel = TIMx_IRQn(self->TIMx),
        .NVIC_PriorityGroup = NVIC_PriorityGroup_2,
        .NVIC_IRQChannelPreemptionPriority = 0,
        .NVIC_IRQChannelSubPriority = 2,
    };
    TIM_Interrupt_Init(&TIM_interrupt);
}

void TIM_InternalClock(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_InternalClockConfig(TIMx);
}
void TIM_ETRClockMode2(TIM_TypeDef *TIMx, ClockSourceConfig_t *config) {
    TIM_ETRClockMode2Config(TIMx, config->TIM_ExtTRGPrescaler,
                            config->TIM_ExtTRGPolarity, config->ExtTRGFilter);
};