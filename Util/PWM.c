#include <string.h>

#include "Compare.h"
#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *self) {
    if (self->TIM_Init) {
        TIM_t tim = {
            .TIMx = self->TIMx,
            .ClockSource = TIM_InternalClock,
            .Prescaler = self->Prescaler,
            .Period = self->Period,
            .Cmd = ENABLE,
        };
        TIM_Init(&tim, NULL);
    }

    uint8_t count = 0;
    char *temp = self->Channel;
    do {
        Compare_t compare = {
            .TIMx = self->TIMx,
            .Pulse = 0,
            .TIM_OCInit = TIM_OCxInit(temp[0] - '0'),
            .TIM_SetCompare = TIM_SetComparex(temp[0] - '0'),
        };
        Compare_Init(&compare);
        self->TIM_SetCompare[count] = compare.TIM_SetCompare;
    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2) &&
             (count = count + 1));
}

void PWM_SetPrescaler(PWM_t *self, uint16_t prescaler) {
    TIM_PrescalerConfig(self->TIMx, prescaler, TIM_PSCReloadMode_Immediate);
}

void PWM_SetPulse(PWM_t *self, uint8_t channel, uint16_t pulse) {
    self->TIM_SetCompare[channel - 1](self->TIMx, pulse);
}