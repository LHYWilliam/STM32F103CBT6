#include "main.h"

static void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu);
static void OLED_ShowTextPage(OLED_t *OLED, TextPage_t *Page);
static void OLED_ShowMQxText(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu);

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    if ((ImageMenu_t *)Menu == &ImageMenu) {
        OLED_ShowImageMenu(&OLED, Menu);

    } else if ((TextMenu_t *)Menu == &TextMenu) {
        OLED_ShowTextMenu(&OLED, Menu);
    }

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);
}

static void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu) {
    if (Menu->Page == MQ2TextPage) {
        OLED_ShowMQxPage(OLED, MQ2TextPage, &MQ2Sensor);

    } else if (Menu->Page == MQ3TextPage) {
        OLED_ShowMQxPage(OLED, MQ3TextPage, &MQ3Sensor);

    } else if (Menu->Page == MQ7TextPage) {
        OLED_ShowMQxPage(OLED, MQ7TextPage, &MQ7Sensor);

    } else if (Menu->Page == MQ135TextPage) {
        OLED_ShowMQxPage(OLED, MQ135TextPage, &MQ135Sensor);

    } else {
        OLED_ShowTextPage(OLED, Menu->Page);
        OLED_ShowSelectioneBar(OLED, &Bar);
    }
}

static void OLED_ShowTextPage(OLED_t *OLED, TextPage_t *Page) {
    if (Page->TitleY + Page->TitleHeight >= 0) {
        OLED_Printf(OLED, Page->TitleX, Page->TitleY, Page->Title);
    }

    for (uint8_t i = 0; i < Page->NumOfLowerPages; i++) {
        if (Page->LowerPages[i].Y + Page->LowerPages[i].Height < 0) {
            continue;
        }
        if (Page->LowerPages[i].Y + Page->LowerPages[i].Height >=
            OLED->Height) {
            break;
        }

        if (&Page->LowerPages[i] == MQ2TextPage) {
            OLED_ShowMQxText(OLED, MQ2TextPage, &MQ2Sensor);

        } else if (&Page->LowerPages[i] == MQ3TextPage) {
            OLED_ShowMQxText(OLED, MQ3TextPage, &MQ3Sensor);

        } else if (&Page->LowerPages[i] == MQ7TextPage) {
            OLED_ShowMQxText(OLED, MQ7TextPage, &MQ7Sensor);

        } else if (&Page->LowerPages[i] == MQ135TextPage) {
            OLED_ShowMQxText(OLED, MQ135TextPage, &MQ135Sensor);

        } else if (Page == SettingTextPage) {
            OLED_Printf(OLED, Page->LowerPages[i].X, Page->LowerPages[i].Y,
                        "%s", Page->LowerPages[i].Title);
            OLED_ShowImage(
                OLED, OLED->Width - 1 - OLED->FontWidth * 6 - 8,
                Page->LowerPages[i].Y, 8, 8,
                SettingImage[SettingTextPage->LowerPages[i].Setting]);

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

static void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu) {
    for (uint8_t i = 0; i < ImageMenu.NumOfPages; i++) {
        if (ImageMenu.Page[i].ImageX + ImageMenu.ImageWidth < 0) {
            continue;
        }
        if (ImageMenu.Page[i].ImageX >= OLED->Width) {
            break;
        }

        OLED_ShowImage(OLED, ImageMenu.Page[i].ImageX, ImageMenu.Page[i].ImageY,
                       ImageMenu.ImageWidth, ImageMenu.ImageHeight,
                       ImageMenu.Page[i].Image);

        OLED_Printf(OLED, ImageMenu.Page[i].TitleX, ImageMenu.Page[i].TitleY,
                    "%s", ImageMenu.Page[i].Title);
    }

    OLED_ShowSelectioneBar(OLED, &Bar);
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
}

void vUpdateTimerCallback(TimerHandle_t pxTimer) {
    static uint8_t Counter = 0;

    if ((ImageMenu_t *)Menu == &ImageMenu) {
        ImageMenu_Update(Menu, &OLED);
        SelectioneBar_Update(&Bar);

    } else if ((TextMenu_t *)Menu == &TextMenu && Counter % 2) {
        if (((TextMenu_t *)Menu)->Page != MQ2TextPage &&
            ((TextMenu_t *)Menu)->Page != MQ3TextPage &&
            ((TextMenu_t *)Menu)->Page != MQ7TextPage &&
            ((TextMenu_t *)Menu)->Page != MQ135TextPage) {
            TextMenu_Update(Menu, &OLED);
        }

        SelectioneBar_Update(&Bar);
    }

    MQSensor_UpdateState(&MQ2Sensor);
    MQSensor_UpdateState(&MQ3Sensor);
    MQSensor_UpdateState(&MQ7Sensor);
    MQSensor_UpdateState(&MQ135Sensor);

    Counter++;
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            if ((ImageMenu_t *)Menu == &ImageMenu) {
                ImageMenu_CursorDec(Menu);
                SelectioneBar_BindImagePage(
                    &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);

            } else if ((TextMenu_t *)Menu == &TextMenu) {
                if (TextMenu.Page == MQ2TextPage) {
                    MQSensor_UpdateThreshold(&MQ2Sensor, 128);

                } else if (TextMenu.Page == MQ3TextPage) {
                    MQSensor_UpdateThreshold(&MQ3Sensor, 128);

                } else if (TextMenu.Page == MQ7TextPage) {
                    MQSensor_UpdateThreshold(&MQ7Sensor, 128);

                } else if (TextMenu.Page == MQ135TextPage) {
                    MQSensor_UpdateThreshold(&MQ135Sensor, 128);

                } else {
                    if (TextMenu_CursorDec(Menu)) {
                        SelectioneBar_BindTextPage(
                            &Bar,
                            &((TextMenu_t *)Menu)
                                 ->Page
                                 ->LowerPages[((TextMenu_t *)Menu)->Cursor]);
                    }
                }
            }
        }

        if (Key_Read(&KeyDown)) {
            if ((ImageMenu_t *)Menu == &ImageMenu) {
                ImageMenu_CursorInc(Menu);
                SelectioneBar_BindImagePage(
                    &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);

            } else if ((TextMenu_t *)Menu == &TextMenu) {
                if (TextMenu.Page == MQ2TextPage) {
                    MQSensor_UpdateThreshold(&MQ2Sensor, -128);

                } else if (TextMenu.Page == MQ3TextPage) {
                    MQSensor_UpdateThreshold(&MQ3Sensor, -128);

                } else if (TextMenu.Page == MQ7TextPage) {
                    MQSensor_UpdateThreshold(&MQ7Sensor, -128);

                } else if (TextMenu.Page == MQ135TextPage) {
                    MQSensor_UpdateThreshold(&MQ135Sensor, -128);

                } else {
                    if (TextMenu_CursorInc(Menu)) {
                        SelectioneBar_BindTextPage(
                            &Bar,
                            &((TextMenu_t *)Menu)
                                 ->Page
                                 ->LowerPages[((TextMenu_t *)Menu)->Cursor]);
                    }
                }
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if ((ImageMenu_t *)Menu == &ImageMenu) {
                Menu = &TextMenu;
                TextMenu.Page = ImageMenu.Page[ImageMenu.Cursor].TextPage;
                SelectioneBar_BindTextPage(
                    &Bar,
                    &((TextMenu_t *)Menu)->Page->LowerPages[TextMenu.Cursor]);

            } else if ((TextMenu_t *)Menu == &TextMenu) {
                if (TextMenu.Page == SettingTextPage) {
                    TextPage_ReverseSetting(TextMenu.Page);

                } else {
                    if (TextMenu_EnterLowerPage(Menu)) {
                        SelectioneBar_BindTextPage(
                            &Bar, &((TextMenu_t *)Menu)
                                       ->Page->LowerPages[TextMenu.Cursor]);
                    }
                }
            }
        }

        if (Key_Read(&KeyCancel)) {
            if ((ImageMenu_t *)Menu == &ImageMenu) {

            } else if ((TextMenu_t *)Menu == &TextMenu) {
                if (TextMenu.Page == HomeTextPage) {
                    Menu = &ImageMenu;
                    SelectioneBar_BindImagePage(
                        &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);

                } else if (TextMenu_ReturnUpperPage(Menu)) {
                    SelectioneBar_BindTextPage(
                        &Bar, &((TextMenu_t *)Menu)
                                   ->Page->LowerPages[TextMenu.Cursor]);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}