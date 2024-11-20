#include "FreeRTOS.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

extern Key_t KeyUp;
extern Key_t KeyDown;
extern Key_t KeyConfirm;
extern Key_t KeyCancel;
extern LED_t LED;
extern TextMenu_t Menu;
extern Sampler_t Sampler;
extern OLED_t OLED;
extern TextPage_t *ADC_Page;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Turn(&LED); }

void vSamplerTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Height - 1, OLED.Height - OLED.FontHeight - 1,
                "%3d %%", time);
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

    OLED_Printf(&OLED, 1 - 1, OLED.Height - OLED.FontHeight - 1, "%.3f V",
                Sampler.Data[Sampler.Index] * 3.3 / 4095.);

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

void vMenuTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%", time);
    time = xTaskGetTickCount();

    if (Menu.Page == ADC_Page) {
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

        OLED_Printf(&OLED, 1 - 1, OLED.Height - OLED.FontHeight - 1, "%.3f V",
                    Sampler.Data[Sampler.Index] * 3.3 / 4095.);

    } else {
        uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;

        for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {
            if (&Menu.Page->LowerPages[begin + i] == ADC_Page) {
                OLED_Printf(&OLED, 0, i * OLED.FontHeight, "%s %.3f %s",
                            Menu.Page->LowerPages[begin + i].Title,
                            Sampler.Data[Sampler.Index] * 3.3 / 4095.,
                            begin + i == Menu.Cursor ? "<-" : "");
            } else {
                OLED_Printf(&OLED, 0, i * OLED.FontHeight, "%s %s",
                            Menu.Page->LowerPages[begin + i].Title,
                            begin + i == Menu.Cursor ? "<-" : "");
            }
        }
        OLED_SetFont(&OLED, OLEDFont_Chinese16X16);
        OLED_Printf(&OLED, 64, 32, "刘浩元");
        OLED_SetFont(&OLED, OLEDFont_6X8);
    }

    OLED_SendBuffer(&OLED);
    time = xTaskGetTickCount() - time;
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            TextMenu_CursorDec(&Menu);
        }
        if (Key_Read(&KeyDown)) {
            TextMenu_CursorInc(&Menu);
        }
        if (Key_Read(&KeyConfirm)) {
            TextMenu_EnterLowerPage(&Menu);
        }
        if (Key_Read(&KeyCancel)) {
            TextMenu_ReturnUpperPage(&Menu);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}