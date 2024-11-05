#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "Sampler.h"
#include "U8g2.h"

extern LED_t LED;
extern Sampler_t Sampler;
extern U8G2_t U8G2;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Turn(&LED); }

void vU8G2TimerCallback(TimerHandle_t pxTimer) {
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
            U8G2.Height - ((float)Sampler.Data[(Index + 1) % Sampler.Length] *
                               U8G2.Height / 2. / 4095. +
                           U8G2.Height / 4.));

        x++;
        Index = (Index + 1) % Sampler.Length;
    } while (x < U8G2.Width - 1);

    U8G2_Printf(&U8G2, 1 - 1, 64 - 1, "%.3f V",
                Sampler.Data[Sampler.Index] * 3.3 / 4095.);

    u8g2_SendBuffer(&U8G2.u8g2);
}