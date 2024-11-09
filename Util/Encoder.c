#include "Encoder.h"
#include "Capture.h"
#include "GPIO.h"
#include "TIM.h"

void Encoder_Init(Encoder_t *self) {
    GPIO_t GPIO = {
        .Mode = GPIO_Mode_IPU,
    };
    GPIO_InitPin(GPIO, self->GPIOxPiny);

    TIM_t tim = {
        .TIMx = self->TIMx,
        .ClockSource = NULL,
        .Prescaler = 1 - 1,
        .Period = 65536 - 1,
        .Cmd = DISABLE,
    };
    TIM_Init(&tim, NULL);

    Capture_t capture1 = {
        .TIMx = self->TIMx,
        .TIM_Channel = TIM_Channel_1,
        .TIM_ICPolarity = TIM_ICPolarity_Rising,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,
        .TIM_ICFilter = 0xF,
        .TIM_GetCapture = TIM_GetCapture1,
    };
    Capture_Init(&capture1);

    Capture_t capture2 = {
        .TIMx = self->TIMx,
        .TIM_Channel = TIM_Channel_2,
        .TIM_ICPolarity = TIM_ICPolarity_Rising,
        .TIM_ICSelection = TIM_ICSelection_DirectTI,
        .TIM_ICFilter = 0xF,
        .TIM_GetCapture = TIM_GetCapture2,
    };
    Capture_Init(&capture2);

    TIM_EncoderInterfaceConfig(self->TIMx, TIM_EncoderMode_TI12,
                               self->invert ? TIM_ICPolarity_Falling
                                            : TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_Cmd(self->TIMx, ENABLE);
    TIM_ClearFlag(self->TIMx, TIM_FLAG_Update);
}

int16_t Encoder_Get(Encoder_t *encoder) {
    int16_t speed = (int16_t)TIM_GetCounter(encoder->TIMx);
    TIM_SetCounter(encoder->TIMx, 0);

    return speed;
}