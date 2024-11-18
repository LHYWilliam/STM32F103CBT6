#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "OLED.h"
#include "Sampler.h"

extern LED_t LED;
extern Sampler_t Sampler;
extern OLED_t OLED;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Turn(&LED); }

void vU8G2TimerCallback(TimerHandle_t pxTimer) {
    u8g2_ClearBuffer(&OLED.u8g2);

    static uint32_t time;
    u8g2_Printf(&OLED, 64 - 1, 64 - 1, "%3d %%", time);
    time = xTaskGetTickCount();

    uint16_t x = 0;
    uint16_t Index = (Sampler.Index + 1) % Sampler.Length;

    while (x < OLED.Width - 1) {
        u8g2_DrawLine(
            &OLED.u8g2, x * (OLED.Width - 1) / (Sampler.Length - 1),
            OLED.Height - 1 -
                (Sampler.Data[Index] * (OLED.Height - 1) / 2. / 4095. +
                 (OLED.Height - 1) / 4.),
            (x + 1) * (OLED.Width - 1) / (Sampler.Length - 1),
            OLED.Height - 1 -
                (Sampler.Data[(Index + 1) % Sampler.Length] *
                     (OLED.Height - 1) / 2. / 4095. +
                 (OLED.Height - 1) / 4.));

        x++;
        Index = (Index + 1) % Sampler.Length;
    }

    u8g2_Printf(&OLED, 1 - 1, 64 - 1, "%.3f V",
                Sampler.Data[Sampler.Index] * 3.3 / 4095.);

    u8g2_SendBuffer(&OLED.u8g2);

    time = xTaskGetTickCount() - time;
}