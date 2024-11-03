#include <stdlib.h>

#include "Interrupt.h"
#include "TIM.h"
#include "Timer.h"

void Timer_Init(Timer_t *self) {
    TIM_t tim = {
        .TIMx = self->TIMx,
        .ClockSource = TIM_InternalClock,
        .Prescaler = 7200 - 1,
        .Period = self->ms * 10 - 1,
        .Cmd = DISABLE,
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