#include "main.h"

static void OLED_ShowHomePage(OLED_t *OLED, TextMenu_t *Menu);
static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t Y, uint8_t i);
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
        uint8_t begin = Menu.Cursor >= Menu.TextCountOfHomePage
                            ? Menu.Cursor - (Menu.TextCountOfOtherPage - 1)
                            : 0;
        for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {

            OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2), "%s%s%s",
                        begin + i == Menu.Cursor ? ">" : ".",
                        Menu.Page->LowerPages[begin + i].Title,
                        begin + i == Menu.Cursor ? "<" : "");
        }
    }

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
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
            if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, 128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, 128);

            } else {
                TextMenu_CursorDec(&Menu);
            }
        }

        if (Key_Read(&KeyDown)) {
            if (Menu.Page == MQ3Page) {
                MQSensor_UpdateThreshold(&MQ3, -128);

            } else if (Menu.Page == MQ135Page) {
                MQSensor_UpdateThreshold(&MQ135, -128);

            } else {
                TextMenu_CursorInc(&Menu);
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu.Page == SettingPage) {
                SettingPage->LowerPages[Menu.Cursor].Setting =
                    !SettingPage->LowerPages[Menu.Cursor].Setting;

            } else {
                TextMenu_EnterLowerPage(&Menu);
            }
        }

        if (Key_Read(&KeyCancel)) {
            TextMenu_ReturnUpperPage(&Menu);
        }

        if (ResetSetting->Setting) {
            __NVIC_SystemReset();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void OLED_ShowHomePage(OLED_t *OLED, TextMenu_t *Menu) {
    Menu->PageNumber = TextMenu_PageNumber(Menu);

    if (Menu->Page->NumOfLowerPages) {
        if (Menu->PageNumber < 1) {
            TextMenu_Update(Menu, 0);

        } else {
            TextMenu_Update(Menu,
                            Menu->Page->Y -
                                (Menu->Page
                                     ->LowerPages[Menu->TextCountOfHomePage +
                                                  Menu->TextCountOfOtherPage *
                                                      (Menu->PageNumber - 1)]
                                     .Y -
                                 Menu->Speed));
        }
    }

    if (Menu->Cursor < Menu->TextCountOfHomePage) {
        OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        OLED_Printf(OLED, Menu->Page->X, Menu->Page->Y, Menu->Page->Title);
        OLED_SetFont(OLED, OLEDFont_6X8);
    }

    for (uint8_t i = 0; i < Menu->Page->NumOfLowerPages; i++) {
        if (Menu->Page->LowerPages[i].Y < 0) {
            continue;
        }
        if (Menu->Page->LowerPages[i].Y + Menu->Page->Space > OLED->Height) {
            break;
        }

        if (&Menu->Page->LowerPages[i] == MQ3Page) {
            OLED_ShowMQxText(OLED, Menu, MQ3Page, &MQ3,
                             Menu->Page->LowerPages[i].Y, i);

        } else if (&Menu->Page->LowerPages[i] == MQ135Page) {
            OLED_ShowMQxText(OLED, Menu, MQ135Page, &MQ135,
                             Menu->Page->LowerPages[i].Y, i);

        } else {
            OLED_Printf(OLED, 0, Menu->Page->LowerPages[i].Y, "%s",
                        Menu->Page->LowerPages[i].Title);
        }
    }

    SelectioneBar_Update(
        &Menu->Bar, Menu->Page->LowerPages[Menu->Cursor].Y - 1,
        OLED->FontWidth * strlen(Menu->Page->LowerPages[Menu->Cursor].Title) +
            1,
        OLED->FontHeight + 1);
    OLED_ShowSelectioneBar(OLED, &Menu->Bar);
}

static void OLED_ShowMQxText(OLED_t *OLED, TextMenu_t *Menu,
                             TextPage_t *MQxPage, MQSensor_t *MQSensor,
                             uint8_t Y, uint8_t i) {
    OLED_Printf(OLED, 0, Y, "%-6s", Menu->Page->LowerPages[i].Title);
    OLED_Printf(OLED, OLED->Width - OLED->FontWidth * 12, Y, "%.3f %6s",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]),
                MQSensor->State ? "Danger" : "Safe");
}

static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor) {
    for (uint8_t x = 0, Index = (MQSensor->Index + 1) % MQSensor->Length;
         x < OLED->Width - 1; x++, Index = (Index + 1) % MQSensor->Length) {
        OLED_DrawLine(
            OLED, x * (OLED->Width - 1) / (MQSensor->Length - 1),
            ADCToOLED(MQSensor->Data[Index]),
            (x + 1) * (OLED->Width - 1) / (MQSensor->Length - 1),
            ADCToOLED(MQSensor->Data[(Index + 1) % MQSensor->Length]));
    }

    OLED_DrawHLine(OLED, 0, ADCToOLED(MQSensor->Threshold), OLED->Width - 1, 1,
                   2);

    OLED_Printf(OLED, 1 - 1, 1 - 1, "%S %s", MQxPage->Title,
                MQSensor->State ? "Danger" : "Safe");

    OLED_Printf(OLED, 1 - 1, OLED->Height - OLED->FontHeight - 1, "%.3f V",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]));
}

static void OLED_ShowSettingPage(OLED_t *OLED, TextMenu_t *Menu,
                                 TextPage_t *SettingPage) {
    Menu->PageNumber = TextMenu_PageNumber(Menu);

    if (Menu->Page->NumOfLowerPages) {
        if (Menu->PageNumber < 1) {
            TextMenu_Update(Menu, 0);

        } else {
            TextMenu_Update(Menu,
                            Menu->Page->Y -
                                (Menu->Page
                                     ->LowerPages[Menu->TextCountOfHomePage +
                                                  Menu->TextCountOfOtherPage *
                                                      (Menu->PageNumber - 1)]
                                     .Y -
                                 Menu->Speed));
        }
    }

    if (Menu->Cursor < Menu->TextCountOfHomePage) {
        OLED_Printf(OLED, Menu->Page->X, Menu->Page->Y, Menu->Page->Title);
    }

    for (uint8_t i = 0; i < Menu->Page->NumOfLowerPages; i++) {
        if (Menu->Page->LowerPages[i].Y < 0) {
            continue;
        }
        if (Menu->Page->LowerPages[i].Y + Menu->Page->Space > OLED->Height) {
            break;
        }

        OLED_Printf(OLED, OLED->Width - OLED->FontWidth * 7,
                    Menu->Page->LowerPages[i].Y, "%7s",
                    SettingPage->LowerPages[i].Setting ? "Enable" : "Disable");
        OLED_Printf(OLED, 0, Menu->Page->LowerPages[i].Y, "%s",
                    Menu->Page->LowerPages[i].Title);
    }

    SelectioneBar_Update(
        &Menu->Bar, Menu->Page->LowerPages[Menu->Cursor].Y - 1,
        OLED->FontWidth * strlen(Menu->Page->LowerPages[Menu->Cursor].Title) +
            1,
        OLED->FontHeight + 1);
    OLED_ShowSelectioneBar(OLED, &Menu->Bar);
}