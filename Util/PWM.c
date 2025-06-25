#include <string.h>

#include "Compare.h"
#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *Self) {
    if (Self->TIM_Init) {
        TIM_t tim = {
            .TIMx        = Self->TIMx,
            .ClockSource = TIM_InternalClock,
            .Prescaler   = Self->Prescaler,
            .Period      = Self->Period,
            .Cmd         = ENABLE,
        };
        TIM_Init(&tim, NULL);
    }

    for (uint8_t channel = 1; channel <= Self->NbrOfChannel; channel++) {
        Compare_t compare = {
            .TIMx           = Self->TIMx,
            .TIM_OCInit     = TIM_OCxInit(Self->Channel[channel - 1]),
            .TIM_SetCompare = TIM_SetComparex(Self->Channel[channel - 1]),
        };
        Compare_Init(&compare);

        Self->TIM_SetCompare[channel - 1] = compare.TIM_SetCompare;
    }
}

void PWM_SetPrescaler(PWM_t *Self, uint16_t Prescaler) {
    TIM_PrescalerConfig(Self->TIMx, Prescaler, TIM_PSCReloadMode_Immediate);
}

void PWM_SetPulse(PWM_t *Self, uint8_t Channel, uint16_t Pulse) {
    Self->TIM_SetCompare[Channel - 1](Self->TIMx, Pulse);
}