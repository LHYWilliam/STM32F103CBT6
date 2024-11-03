#include <stdlib.h>

#include "Interrupt.h"
#include "TIM.h"
#include "Timer.h"

void Timer_Init(Timer_t *self) {
    TIM_t tim = {
        .TIMx = self->TIMx,
        .ClockSource = TIM_InternalClock,
        .Prescaler = self->ms   ? 7200 - 1
                     : self->Hz ? (uint16_t)(72000000. / 65535. - 1)
                                : NULL,
        .Period = self->ms   ? self->ms * 10 - 1
                  : self->Hz ? (uint16_t)(65535. / self->Hz - 1)
                             : NULL,
        .Cmd = self->Interrupt ? DISABLE : ENABLE,
        .TRGO = self->TRGO,
    };
    TIM_Init(&tim, NULL);

    if (self->Interrupt) {
        TIMInterrupt_t TIM_interrupt = {
            .TIMx = self->TIMx,
            .NVIC_IRQChannel = TIMx_IRQn(self->TIMx),
            .NVIC_PriorityGroup = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = self->Priority,
        };
        TIM_Interrupt_Init(&TIM_interrupt);
    }
}