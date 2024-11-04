#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "Sampler.h"
#include "Timer.h"
#include "u8g2.h"

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = HIGH,
};

Key_t Key = {
    .GPIOxPiny = A0,
    .Mode = HIGH,
};

U8G2_t U8G2 = {
    .SCL = B8,
    .SDA = B9,
    .Width = 128,
    .Height = 64,
};

#define LENGTH 64
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1",
    .GPIOxPiny = A1,

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

Timer_t Timer = {
    .TIMx = TIM2,
    .ms = 1000,
    .Interrupt = ENABLE,
    .Priority = 15,
};

int main() {
    LED_Init(&LED);
    Key_Init(&Key);

    U8G2_Init(&U8G2);
    Sampler_Init(&Sampler);
    Timer_Init(&Timer);

    u8g2_SetFont(&U8G2.u8g2, u8g2_font_t0_16_me);
    for (;;) {
        u8g2_ClearBuffer(&U8G2.u8g2);

        uint16_t x = 0;
        uint16_t Index = (Sampler.Index + 1) % Sampler.Length;
        do {
            u8g2_DrawLine(
                &U8G2.u8g2, (float)x * U8G2.Width / Sampler.Length,
                U8G2.Height -
                    ((float)Sampler.Data[Index] * U8G2.Height / 2. / 4095. +
                     U8G2.Height / 4.),
                (float)(x + 1) * U8G2.Width / Sampler.Length,
                U8G2.Height -
                    ((float)Sampler.Data[(Index + 1) % Sampler.Length] *
                         U8G2.Height / 2. / 4095. +
                     U8G2.Height / 4.));

            x++;
            Index = (Index + 1) % Sampler.Length;
        } while (x < U8G2.Width - 1);

        U8G2_Printf(&U8G2, 1 - 1, 64 - 1, "%.3f V",
                    Sampler.Data[Sampler.Index] * 3.3 / 4095.);

        u8g2_SendBuffer(&U8G2.u8g2);
    }
}

void TIM2_IRQHandler() {
    if (TIM_GetITStatus(Timer.TIMx, TIM_IT_Update)) {
        LED_Turn(&LED);

        TIM_ClearITPendingBit(Timer.TIMx, TIM_IT_Update);
    }
}

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        Sampler.Index = (Sampler.Index + 1) % Sampler.Length;

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}