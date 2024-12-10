#include "Encoder.h"
#include "Capture.h"
#include "TIM.h"

void Encoder_Init(Encoder_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_IPU;
    GPIO_InitPin(&GPIO, self->GPIOxPiny[0]);
    GPIO_InitPin(&GPIO, self->GPIOxPiny[1]);

    TIM_t TIM = {
        .TIMx = self->TIMx,
        .Prescaler = 1 - 1,
        .Period = 65536 - 1,
    };
    TIM_Init(&TIM, NULL);

    Capture_t Capture = {
        .TIMx = self->TIMx,
        .TIM_ICPolarity = TIM_ICPolarity_Rising,
    };

    Capture.TIM_Channel = TIM_Channel_1;
    Capture_Init(&Capture);

    Capture.TIM_Channel = TIM_Channel_2;
    Capture_Init(&Capture);

    TIM_EncoderInterfaceConfig(self->TIMx, TIM_EncoderMode_TI12,
                               self->Invert ? TIM_ICPolarity_Falling
                                            : TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);

    TIM_Cmd(self->TIMx, ENABLE);
    TIM_ClearFlag(self->TIMx, TIM_FLAG_Update);
}

int16_t Encoder_GetCount(Encoder_t *self) {
    int16_t speed = (int16_t)TIM_GetCounter(self->TIMx);
    TIM_SetCounter(self->TIMx, 0);

    return speed;
}