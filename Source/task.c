#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

#define MQxDataLength 128

extern LED_t LED;

extern Sampler_t Sampler;

extern TextMenu_t Menu;
extern OLED_t OLED;

extern TextPage_t *HomePage;
extern TextPage_t *MQ3Page;
extern TextPage_t *MQ135Page;

extern LED_t MQ3_LED;
extern LED_t MQ135_LED;
extern Key_t KeyUp;
extern Key_t KeyDown;
extern Key_t KeyConfirm;
extern Key_t KeyCancel;

extern int16_t MQ3_Index;
extern uint16_t MQ3_Data[128];

extern int16_t MQ135_Index;
extern uint16_t MQ135_Data[128];

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    if (MQ3_Data[MQ3_Index] > 2048 + 256) {
        LED_On(&MQ3_LED);
    } else if (MQ3_Data[MQ3_Index] < 2048 - 256) {
        LED_Off(&MQ3_LED);
    }

    if (MQ135_Data[MQ135_Index] > 2048 + 256) {
        LED_On(&MQ135_LED);
    } else if (MQ135_Data[MQ135_Index] < 2048 - 256) {
        LED_Off(&MQ135_LED);
    }

    LED_Turn(&LED);
}

void vSamplerTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Height - 1, OLED.Height - OLED.FontHeight - 1,
                "%3d %%", time);
    time = xTaskGetTickCount();

    for (uint16_t x = 0, Index = (MQ3_Index + 1) % MQxDataLength;
         x < OLED.Width - 1; x++, Index = (Index + 1) % MQxDataLength) {
        OLED_DrawLine(&OLED, x,
                      OLED.Height - 1 -
                          (MQ3_Data[Index] * (OLED.Height - 1) / 2. / 4095. +
                           (OLED.Height - 1) / 4.),
                      x + 1,
                      OLED.Height - 1 -
                          (MQ3_Data[(Index + 1) % MQxDataLength] *
                               (OLED.Height - 1) / 2. / 4095. +
                           (OLED.Height - 1) / 4.));
    }

    OLED_Printf(&OLED, 1 - 1, OLED.Height - OLED.FontHeight - 1, "%.3f V",
                MQ3_Data[MQ3_Index] * 3.3 / 4095.);

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%", time);
    time = xTaskGetTickCount();

    if (Menu.Page == MQ3Page) {
        uint16_t x = 0;
        uint16_t Index = (MQ3_Index + 1) % MQxDataLength;

        while (x < OLED.Width - 1) {
            OLED_DrawLine(
                &OLED, x * (OLED.Width - 1) / (MQxDataLength - 1),
                OLED.Height - 1 -
                    (MQ3_Data[Index] * (OLED.Height - 1) / 2. / 4095. +
                     (OLED.Height - 1) / 4.),
                (x + 1) * (OLED.Width - 1) / (MQxDataLength - 1),
                OLED.Height - 1 -
                    (MQ3_Data[(Index + 1) % MQxDataLength] * (OLED.Height - 1) /
                         2. / 4095. +
                     (OLED.Height - 1) / 4.));

            x++;
            Index = (Index + 1) % MQxDataLength;
        }

        OLED_Printf(&OLED, 1 - 1, 1 - 1, "%S %s", MQ3Page->Title,
                    GPIO_ReadInput(MQ3_LED.ODR) == MQ3_LED.Mode ? "Danger"
                                                                : "Safe");

        OLED_Printf(&OLED, 1 - 1, OLED.Height - OLED.FontHeight - 1, "%.3f V",
                    MQ3_Data[MQ3_Index] * 3.3 / 4095.);

    } else if (Menu.Page == MQ135Page) {
        uint16_t x = 0;
        uint16_t Index = (MQ135_Index + 1) % MQxDataLength;

        while (x < OLED.Width - 1) {
            OLED_DrawLine(
                &OLED, x * (OLED.Width - 1) / (MQxDataLength - 1),
                OLED.Height - 1 -
                    (MQ135_Data[Index] * (OLED.Height - 1) / 2. / 4095. +
                     (OLED.Height - 1) / 4.),
                (x + 1) * (OLED.Width - 1) / (MQxDataLength - 1),
                OLED.Height - 1 -
                    (MQ135_Data[(Index + 1) % MQxDataLength] *
                         (OLED.Height - 1) / 2. / 4095. +
                     (OLED.Height - 1) / 4.));

            x++;
            Index = (Index + 1) % MQxDataLength;
        }

        OLED_Printf(&OLED, 1 - 1, 1 - 1, "%S %s", MQ135Page->Title,
                    GPIO_ReadInput(MQ135_LED.ODR) == MQ135_LED.Mode ? "Danger"
                                                                    : "Safe");

        OLED_Printf(&OLED, 1 - 1, OLED.Height - OLED.FontHeight - 1, "%.3f V",
                    MQ135_Data[MQ135_Index] * 3.3 / 4095.);

    } else {
        if (Menu.Page == HomePage) {
            OLED_SetFont(&OLED, OLEDFont_Chinese12X12);
            OLED_Printf(&OLED, 10 + 1 - 1, 1 - 1, "异味检测与开窗系统");
            OLED_SetFont(&OLED, OLEDFont_6X8);
        }

        uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;

        for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {
            if (&Menu.Page->LowerPages[begin + i] == MQ3Page) {
                OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2),
                            "%s%-6s %.3f%s",
                            begin + i == Menu.Cursor ? ">" : ".",
                            Menu.Page->LowerPages[begin + i].Title,
                            MQ3_Data[MQ3_Index] * 3.3 / 4095.,
                            begin + i == Menu.Cursor ? "<" : "");
                OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 6,
                            20 + i * (OLED.FontHeight + 2), "%6s",
                            GPIO_ReadInput(MQ3_LED.ODR) == MQ3_LED.Mode
                                ? "Danger"
                                : "Safe");

            } else if (&Menu.Page->LowerPages[begin + i] == MQ135Page) {
                OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2),
                            "%s%6s %.3f%s",
                            begin + i == Menu.Cursor ? ">" : ".",
                            Menu.Page->LowerPages[begin + i].Title,
                            MQ135_Data[MQ135_Index] * 3.3 / 4095.,
                            begin + i == Menu.Cursor ? "<" : "");
                OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 6,
                            20 + i * (OLED.FontHeight + 2), "%6s",
                            GPIO_ReadInput(MQ135_LED.ODR) == MQ135_LED.Mode
                                ? "Danger"
                                : "Safe");

            } else {
                OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2),
                            "%s %s %s", begin + i == Menu.Cursor ? ">" : "-",
                            Menu.Page->LowerPages[begin + i].Title,
                            begin + i == Menu.Cursor ? "<-" : "");
            }
        }
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