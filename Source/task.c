#include "main.h"

static void OLED_ShowTextPage(OLED_t *OLED, TextMenu_t *Menu);
static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t i);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%",
                (uint8_t)(time / 10.0 * 100.0));
    time = xTaskGetTickCount();

    if (Menu.Page == MQ3Page) {
        OLED_ShowMQxPage(&OLED, MQ3Page, &MQ3);

    } else if (Menu.Page == MQ135Page) {
        OLED_ShowMQxPage(&OLED, MQ135Page, &MQ135);

    } else {
        OLED_ShowTextPage(&OLED, &Menu);
    }

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

static void OLED_ShowTextPage(OLED_t *OLED, TextMenu_t *Menu) {
    TextPage_Update(Menu->Page, Menu);
    SelectioneBar_Update(&Menu->Bar);

    if (Menu->Page->TitleY >= 0) {
        if (IsChinese(Menu->Page->Title)) {
            OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        }
        OLED_Printf(OLED, Menu->Page->TitleX, Menu->Page->TitleY,
                    Menu->Page->Title);
        OLED_SetFont(OLED, OLEDFont_6X8);
    }

    for (uint8_t i = 0; i < Menu->Page->NumOfLowerPages; i++) {
        if (Menu->Page->LowerPages[i].Y < 0) {
            continue;
        }
        if (Menu->Page->LowerPages[i].Y + Menu->Page->LowerPages[i].Height >
            OLED->Height) {
            break;
        }

        if (&Menu->Page->LowerPages[i] == MQ3Page) {
            OLED_ShowMQxText(OLED, Menu, MQ3Page, &MQ3, i);

        } else if (&Menu->Page->LowerPages[i] == MQ135Page) {
            OLED_ShowMQxText(OLED, Menu, MQ135Page, &MQ135, i);

        } else if (Menu->Page == SettingPage) {
            OLED_Printf(OLED, Menu->Page->LowerPages[i].X,
                        Menu->Page->LowerPages[i].Y, "%s",
                        Menu->Page->LowerPages[i].Title);
            OLED_ShowImage(OLED, OLED->Width - 1 - OLED->FontWidth * 6 - 8,
                           Menu->Page->LowerPages[i].Y, 8, 8,
                           SettingImage[SettingPage->LowerPages[i].Setting]);

        } else {
            OLED_Printf(OLED, Menu->Page->LowerPages[i].X,
                        Menu->Page->LowerPages[i].Y, "%s",
                        Menu->Page->LowerPages[i].Title);
        }
    }

    OLED_ShowSelectioneBar(OLED, &Menu->Bar);
}

static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t i) {
    OLED_Printf(OLED, Menu->Page->LowerPages[i].X, Menu->Page->LowerPages[i].Y,
                "%-6s", Menu->Page->LowerPages[i].Title);
    OLED_Printf(OLED, OLED->Width - 1 - OLED->FontWidth * 12,
                Menu->Page->LowerPages[i].Y, "%.3f %6s",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]),
                MQSensor->State ? "Danger" : "Safe");
}

static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor) {

    OLED_ShowChart(OLED, MQxPage->TitleX, MQxPage->TitleY, MQxPage->TitleWidth,
                   MQxPage->TitleHeight, MQSensor->Data, MQSensor->Length,
                   MQSensor->Index);
    OLED_DrawHLine(
        OLED, MQxPage->TitleX,
        OLED_ADCToY(MQSensor->Threshold, MQxPage->TitleY, MQxPage->TitleHeight),
        MQxPage->TitleWidth, 2);

    OLED_Printf(OLED, 1 - 1, 1 - 1, "%s %s", MQxPage->Title,
                MQSensor->State ? "Danger" : "Safe");
    OLED_Printf(OLED, 1 - 1, OLED->Height - OLED->FontHeight - 1, "%.3f V",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]));
}

void vStateTimerCallback(TimerHandle_t pxTimer) {
    if (StatusLEDSetting->Setting) {
        LED_Turn(&LED);
    } else {
        LED_Off(&LED);
    }

    MQSensor_UpdateState(&MQ3);
    MQSensor_UpdateState(&MQ135);
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, 128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, 128);

            } else {
                if (TextMenu_CursorDec(&Menu)) {
                    SelectioneBar_Bind(&Menu.Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyDown)) {
            if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, -128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, -128);

            } else {
                if (TextMenu_CursorInc(&Menu)) {
                    SelectioneBar_Bind(&Menu.Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu.Page == SettingPage) {
                TextPage_ReverseSetting(Menu.Page);

            } else {
                if (TextMenu_EnterLowerPage(&Menu)) {
                    SelectioneBar_Bind(&Menu.Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyCancel)) {
            if (TextMenu_ReturnUpperPage(&Menu)) {
                SelectioneBar_Bind(&Menu.Bar,
                                   &Menu.Page->LowerPages[Menu.Cursor]);
            }
        }

        if (RestartSetting->Setting) {
            __NVIC_SystemReset();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}