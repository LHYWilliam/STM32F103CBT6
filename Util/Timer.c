#include <stdlib.h>

#include "Interrupt.h"
#include "TIM.h"
#include "Timer.h"

void Timer_Init(Timer_t *Self) {
    TIM_t tim = {
        .TIMx        = Self->TIMx,
        .ClockSource = TIM_InternalClock,
        .Prescaler   = Self->ms   ? 7200 - 1
                       : Self->Hz ? (uint16_t)(72000000. / 65535. - 1)
                                  : NULL,
        .Period      = Self->ms   ? Self->ms * 10 - 1
                       : Self->Hz ? (uint16_t)(65535. / Self->Hz - 1)
                                  : NULL,
        .Cmd         = Self->Interrupt ? DISABLE : ENABLE,
        .TRGO        = Self->TRGO,
    };
    TIM_Init(&tim, NULL);

    if (Self->Interrupt) {
        TIMInterrupt_t TIM_interrupt = {
            .TIMx                              = Self->TIMx,
            .NVIC_IRQChannel                   = TIMx_IRQn(Self->TIMx),
            .NVIC_PriorityGroup                = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = Self->Priority,
        };
        TIM_Interrupt_Init(&TIM_interrupt);
    }
}