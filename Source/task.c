#include "main.h"

static void OLED_ShowTextPage(OLED_t *OLED, TextPage_t *Page);
static void OLED_ShowMQxText(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    // OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
    //             OLED.Height - OLED.FontHeight - 1, "%2d %%",
    //             (uint8_t)(time / 10.0 * 100.0));
    time = xTaskGetTickCount();

    if (Menu.Page == MQ2Page) {
        OLED_ShowMQxPage(&OLED, MQ2Page, &MQ2);

    } else if (Menu.Page == MQ3Page) {
        OLED_ShowMQxPage(&OLED, MQ3Page, &MQ3);

    } else if (Menu.Page == MQ7Page) {
        OLED_ShowMQxPage(&OLED, MQ7Page, &MQ7);

    } else if (Menu.Page == MQ135Page) {
        OLED_ShowMQxPage(&OLED, MQ135Page, &MQ135);

    } else {
        TextMenu_Update(&Menu, &OLED);
        OLED_ShowTextPage(&OLED, Menu.Page);
        SelectioneBar_Update(&Bar);
        OLED_ShowSelectioneBar(&OLED, &Bar);
    }

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

static void OLED_ShowTextPage(OLED_t *OLED, TextPage_t *Page) {
    if (Page->TitleY >= 0) {
        OLED_Printf(OLED, Page->TitleX, Page->TitleY, Page->Title);
    }

    for (uint8_t i = 0; i < Page->NumOfLowerPages; i++) {
        if (Page->LowerPages[i].Y < 0) {
            continue;
        }
        if (Page->LowerPages[i].Y + Page->LowerPages[i].Height >=
            OLED->Height) {
            break;
        }

        if (&Page->LowerPages[i] == MQ2Page) {
            OLED_ShowMQxText(OLED, MQ2Page, &MQ2);

        } else if (&Page->LowerPages[i] == MQ3Page) {
            OLED_ShowMQxText(OLED, MQ3Page, &MQ3);

        } else if (&Page->LowerPages[i] == MQ7Page) {
            OLED_ShowMQxText(OLED, MQ7Page, &MQ7);

        } else if (&Page->LowerPages[i] == MQ135Page) {
            OLED_ShowMQxText(OLED, MQ135Page, &MQ135);

        } else if (Page == SettingPage) {
            OLED_Printf(OLED, Page->LowerPages[i].X, Page->LowerPages[i].Y,
                        "%s", Page->LowerPages[i].Title);
            OLED_ShowImage(OLED, OLED->Width - 1 - OLED->FontWidth * 6 - 8,
                           Page->LowerPages[i].Y, 8, 8,
                           SettingImage[SettingPage->LowerPages[i].Setting]);

        } else {
            OLED_Printf(OLED, Page->LowerPages[i].X, Page->LowerPages[i].Y,
                        "%s", Page->LowerPages[i].Title);
        }
    }
}

static void OLED_ShowMQxText(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor) {
    OLED_Printf(OLED, MQxPage->X, MQxPage->Y, "%-6s", MQxPage->Title);
    OLED_Printf(OLED, OLED->Width - 1 - OLED->FontWidth * 12, MQxPage->Y,
                "%.3f %6s", ADCToVoltage(MQSensor->Data[MQSensor->Index]),
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

    if (RestartSetting->Setting) {
        __NVIC_SystemReset();
    }

    MQSensor_UpdateState(&MQ2);
    MQSensor_UpdateState(&MQ3);
    MQSensor_UpdateState(&MQ7);
    MQSensor_UpdateState(&MQ135);
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            if (Menu.Page == MQ2Page) {
                MQSensor_UpdateThreshold(&MQ2, 128);

            } else if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, 128);

            } else if (Menu.Page == MQ7Page) {
                MQSensor_UpdateThreshold(&MQ7, 128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, 128);

            } else {
                if (TextMenu_CursorDec(&Menu)) {
                    SelectioneBar_Bind(&Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyDown)) {
            if (Menu.Page == MQ2Page) {
                MQSensor_UpdateThreshold(&MQ2, -128);

            } else if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, -128);

            } else if (Menu.Page == MQ7Page) {
                MQSensor_UpdateThreshold(&MQ7, -128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, -128);

            } else {
                if (TextMenu_CursorInc(&Menu)) {
                    SelectioneBar_Bind(&Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu.Page == SettingPage) {
                TextPage_ReverseSetting(Menu.Page);

            } else {
                if (TextMenu_EnterLowerPage(&Menu)) {
                    SelectioneBar_Bind(&Bar,
                                       &Menu.Page->LowerPages[Menu.Cursor]);
                }
            }
        }

        if (Key_Read(&KeyCancel)) {
            if (TextMenu_ReturnUpperPage(&Menu)) {
                SelectioneBar_Bind(&Bar, &Menu.Page->LowerPages[Menu.Cursor]);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}