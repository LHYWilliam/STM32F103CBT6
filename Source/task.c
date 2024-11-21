#include "main.h"

static void OLED_ShowHomePage(OLED_t *OLED);
static void OLED_ShowMQxMenu(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor, uint8_t begin, uint8_t i);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    MQSensor_UpdateState(&MQ3);
    MQSensor_UpdateState(&MQ135);

    LED_Turn(&LED);
}

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%", time);
    time = xTaskGetTickCount();

    if (Menu.Page == HomePage) {
        OLED_ShowHomePage(&OLED);

    } else if (Menu.Page == MQ3Page) {
        OLED_ShowMQxPage(&OLED, MQ3Page, &MQ3);

    } else if (Menu.Page == MQ135Page) {
        OLED_ShowMQxPage(&OLED, MQ135Page, &MQ135);

    } else {
        uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;
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

static void OLED_ShowHomePage(OLED_t *OLED) {
    OLED_SetFont(OLED, OLEDFont_Chinese12X12);
    OLED_Printf(OLED, 10 + 1 - 1, 1 - 1, "异味检测与开窗系统");
    OLED_SetFont(OLED, OLEDFont_6X8);

    uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;
    for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {
        if (&Menu.Page->LowerPages[begin + i] == MQ3Page) {
            OLED_ShowMQxMenu(OLED, MQ3Page, &MQ3, begin, i);

        } else if (&Menu.Page->LowerPages[begin + i] == MQ135Page) {
            OLED_ShowMQxMenu(OLED, MQ135Page, &MQ135, begin, i);

        } else {
            OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s%s%s",
                        begin + i == Menu.Cursor ? ">" : ".",
                        Menu.Page->LowerPages[begin + i].Title,
                        begin + i == Menu.Cursor ? "<" : "");
        }
    }
}

static void OLED_ShowMQxMenu(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor, uint8_t begin, uint8_t i) {
    OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s%-6s %.3f%s",
                begin + i == Menu.Cursor ? ">" : ".",
                Menu.Page->LowerPages[begin + i].Title,
                MQSensor->Data[MQSensor->Index] * 3.3 / 4095.,
                begin + i == Menu.Cursor ? "<" : "");
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
                MQSensor->Data[MQSensor->Index] * 3.3 / 4095.);
}