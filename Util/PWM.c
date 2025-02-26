#include <string.h>

#include "Compare.h"
#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *self) {
    if (self->TIM_Init) {
        TIM_t tim = {
            .TIMx        = self->TIMx,
            .ClockSource = TIM_InternalClock,
            .Prescaler   = self->Prescaler,
            .Period      = self->Period,
            .Cmd         = ENABLE,
        };
        TIM_Init(&tim, NULL);
    }

    for (uint8_t channel = 1; channel <= self->NbrOfChannel; channel++) {
        Compare_t compare = {
            .TIMx           = self->TIMx,
            .TIM_OCInit     = TIM_OCxInit(self->Channel[channel - 1]),
            .TIM_SetCompare = TIM_SetComparex(self->Channel[channel - 1]),
        };
        Compare_Init(&compare);

        self->TIM_SetCompare[channel - 1] = compare.TIM_SetCompare;
    }
}

void PWM_SetPrescaler(PWM_t *self, uint16_t Prescaler) {
    TIM_PrescalerConfig(self->TIMx, Prescaler, TIM_PSCReloadMode_Immediate);
}

void PWM_SetPulse(PWM_t *self, uint8_t Channel, uint16_t Pulse) {
    self->TIM_SetCompare[Channel - 1](self->TIMx, Pulse);
}