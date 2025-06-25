#include "PWMI.h"

void PWMI_Init(PWMI_t *Self) {
    // GPIO_Init_(Self->gpio); // To update to GPIO_InitPin
    TIM_Init(Self->tim, NULL);
    Capture_Init(Self->frequency);
    Capture_Init(Self->duty);

    TIM_SelectInputTrigger(Self->tim->TIMx, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(Self->tim->TIMx, TIM_SlaveMode_Reset);

    TIM_Cmd(Self->tim->TIMx, ENABLE);
    TIM_ClearFlag(Self->tim->TIMx, TIM_FLAG_Update);
}
uint16_t PWMI_GetFrequency(PWMI_t *Self) {
    return 1000000 /
           (Self->frequency->TIM_GetCapture(Self->frequency->TIMx) + 1);
}
uint16_t PWMI_GetDuty(PWMI_t *Self) {
    return (Self->duty->TIM_GetCapture(Self->tim->TIMx) + 1) * 100 /
           (Self->frequency->TIM_GetCapture(Self->tim->TIMx) + 1);
}