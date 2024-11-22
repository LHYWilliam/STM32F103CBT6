#include "main.h"

static void OLED_ShowHomePage(OLED_t *OLED, TextMenu_t *Menu);
static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t begin, uint8_t i);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowSettingPage(OLED_t *OLED, TextMenu_t *Menu,
                                 TextPage_t *SettingPage);

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Turn(&LED); }

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%",
                (uint8_t)(time / 20.0 * 100.0));
    time = xTaskGetTickCount();

    if (Menu.Page == HomePage) {
        OLED_ShowHomePage(&OLED, &Menu);

    } else if (Menu.Page == MQ3Page) {
        OLED_ShowMQxPage(&OLED, MQ3Page, &MQ3);

    } else if (Menu.Page == MQ135Page) {
        OLED_ShowMQxPage(&OLED, MQ135Page, &MQ135);

    } else if (Menu.Page == SettingPage) {
        OLED_ShowSettingPage(&OLED, &Menu, SettingPage);

    } else {
        uint8_t begin = Menu.Cursor >= Menu.NumOfTexts
                            ? Menu.Cursor - (Menu.NumOfTexts - 1)
                            : 0;
        for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {

            OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2), "%s%s%s",
                        begin + i == Menu.Cursor ? ">" : ".",
                        Menu.Page->LowerPages[begin + i].Title,
                        begin + i == Menu.Cursor ? "<" : "");
        }
    }

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

void vMQSensorTimerCallback(TimerHandle_t pxTimer) {
    MQSensor_UpdateState(&MQ3);
    MQSensor_UpdateState(&MQ135);
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

static void OLED_ShowHomePage(OLED_t *OLED, TextMenu_t *Menu) {
    OLED_SetFont(OLED, OLEDFont_Chinese12X12);
    OLED_Printf(OLED, 10 + 1 - 1, 1 - 1, Menu->Page->Title);
    OLED_SetFont(OLED, OLEDFont_6X8);

    uint8_t begin = Menu->Cursor - Menu->Cursor % Menu->NumOfTexts;
    for (uint8_t i = 0;
         (begin + i < Menu->Page->NumOfLowerPages) && (i < Menu->NumOfTexts);
         i++) {
        if (&Menu->Page->LowerPages[begin + i] == MQ3Page) {
            OLED_ShowMQxText(OLED, Menu, MQ3Page, &MQ3, begin, i);

        } else if (&Menu->Page->LowerPages[begin + i] == MQ135Page) {
            OLED_ShowMQxText(OLED, Menu, MQ135Page, &MQ135, begin, i);

        } else {
            OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s",
                        Menu->Page->LowerPages[begin + i].Title);
        }
    }

    if (Menu->Bar.Speed == 0) {
        SelectioneBar_Init(&Menu->Bar, 0, 20 - 1, OLED->FontWidth * 12 + 1,
                           OLED->FontHeight + 1, 2);
    }

    OLED_ShowSelectioneBar(
        OLED, &Menu->Bar,
        20 + Menu->Cursor % Menu->NumOfTexts * (OLED->FontHeight + 2) - 1,
        OLED->FontWidth * 12 + 1);
}

static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t begin, uint8_t i) {
    OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%-6s %.3f",
                Menu->Page->LowerPages[begin + i].Title,
                ADCToVoltage(MQSensor->Data[MQSensor->Index]));
    OLED_Printf(OLED, OLED->Width - OLED->FontWidth * 6,
                20 + i * (OLED->FontHeight + 2), "%6s",
                MQSensor->State ? "Danger" : "Safe");
}

static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor) {
    for (uint16_t x = 0, Index = (MQSensor->Index + 1) % MQSensor->Length;
         x < OLED->Width - 1; x++, Index = (Index + 1) % MQSensor->Length) {
        OLED_DrawLine(
            OLED, x * (OLED->Width - 1) / (MQSensor->Length - 1),
            OLED->Height - 1 -
                (MQSensor->Data[Index] * (OLED->Height - 1) / 2. / 4095. +
                 (OLED->Height - 1) / 4.),
            (x + 1) * (OLED->Width - 1) / (MQSensor->Length - 1),
            OLED->Height - 1 -
                (MQSensor->Data[(Index + 1) % MQSensor->Length] *
                     (OLED->Height - 1) / 2. / 4095. +
                 (OLED->Height - 1) / 4.));
    }

    OLED_Printf(OLED, 1 - 1, 1 - 1, "%S %s", MQxPage->Title,
                MQSensor->State ? "Danger" : "Safe");

    OLED_Printf(OLED, 1 - 1, OLED->Height - OLED->FontHeight - 1, "%.3f V",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]));
}

static void OLED_ShowSettingPage(OLED_t *OLED, TextMenu_t *Menu,
                                 TextPage_t *SettingPage) {
    OLED_Printf(OLED, 0, 0, "Setting");

    uint8_t begin = Menu->Cursor - Menu->Cursor % Menu->NumOfTexts;
    for (uint8_t i = 0;
         (begin + i < Menu->Page->NumOfLowerPages) && (i < Menu->NumOfTexts);
         i++) {

        OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s",
                    Menu->Page->LowerPages[begin + i].Title);
    }

    OLED_ShowSelectioneBar(
        OLED, &Menu->Bar,
        20 + Menu->Cursor % Menu->NumOfTexts * (OLED->FontHeight + 2) - 1,
        OLED->FontWidth * 7 + 1);
}