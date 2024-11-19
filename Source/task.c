#include "FreeRTOS.h"
#include "GPIO.h"
#include "timers.h"

#include "LED.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

extern LED_t LED;
extern TextMenu_t Menu;
extern Sampler_t Sampler;
extern OLED_t OLED;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Turn(&LED); }

void vSamplerTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, 64 - 1, 64 - 16 - 1, "%3d %%", time);
    time = xTaskGetTickCount();

    uint16_t x = 0;
    uint16_t Index = (Sampler.Index + 1) % Sampler.Length;

    while (x < OLED.Width - 1) {
        OLED_DrawLine(
            &OLED, x * (OLED.Width - 1) / (Sampler.Length - 1),
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

    OLED_Printf(&OLED, 1 - 1, 64 - 16 - 1, "%.3f V",
                Sampler.Data[Sampler.Index] * 3.3 / 4095.);

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

void vMenuTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);
    OLED_ShowTextMenu(&OLED, &Menu);
    OLED_SendBuffer(&OLED);
}