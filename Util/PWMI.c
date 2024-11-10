#include "PWMI.h"

void PWMI_Init(PWMI_t *self) {
    // GPIO_Init_(self->gpio); // To update to GPIO_InitPin
    TIM_Init(self->tim, NULL);
    Capture_Init(self->frequency);
    Capture_Init(self->duty);

    TIM_SelectInputTrigger(self->tim->TIMx, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(self->tim->TIMx, TIM_SlaveMode_Reset);

    TIM_Cmd(self->tim->TIMx, ENABLE);
    TIM_ClearFlag(self->tim->TIMx, TIM_FLAG_Update);
}
uint16_t PWMI_GetFrequency(PWMI_t *self) {
    return 1000000 /
           (self->frequency->TIM_GetCapture(self->frequency->TIMx) + 1);
}
uint16_t PWMI_GetDuty(PWMI_t *self) {
    return (self->duty->TIM_GetCapture(self->tim->TIMx) + 1) * 100 /
           (self->frequency->TIM_GetCapture(self->tim->TIMx) + 1);
}