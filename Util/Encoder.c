#include "Encoder.h"
#include "Capture.h"
#include "TIM.h"

void Encoder_Init(Encoder_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_IPU;
    GPIO_InitPin(&GPIO, Self->GPIOxPiny[0]);
    GPIO_InitPin(&GPIO, Self->GPIOxPiny[1]);

    TIM_t TIM = {
        .TIMx      = Self->TIMx,
        .Prescaler = 1 - 1,
        .Period    = 65536 - 1,
    };
    TIM_Init(&TIM, NULL);

    Capture_t Capture = {
        .TIMx           = Self->TIMx,
        .TIM_ICPolarity = TIM_ICPolarity_Rising,
    };

    Capture.TIM_Channel = TIM_Channel_x[Self->Channel[0]];
    Capture_Init(&Capture);

    Capture.TIM_Channel = TIM_Channel_x[Self->Channel[1]];
    Capture_Init(&Capture);

    TIM_EncoderInterfaceConfig(Self->TIMx, TIM_EncoderMode_TI12,
                               Self->Invert ? TIM_ICPolarity_Falling
                                            : TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_Cmd(Self->TIMx, ENABLE);
    TIM_ClearFlag(Self->TIMx, TIM_FLAG_Update);
}

int16_t Encoder_GetCount(Encoder_t *Self) {
    int16_t speed = (int16_t)TIM_GetCounter(Self->TIMx);
    TIM_SetCounter(Self->TIMx, 0);

    return speed;
}