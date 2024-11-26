#include "main.h"

static void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu);
static void OLED_ShowTextPage(OLED_t *OLED, TextPage_t *Page);
static void OLED_ShowMQxText(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             MQSensor_t *MQSensor);
static void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu);

void *LastMenu;

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    if (Menu == &ImageMenu) {
        OLED_ShowImageMenu(&OLED, Menu);

    } else if (Menu == &TextMenu) {
        OLED_ShowTextMenu(&OLED, Menu);
    }

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);
}

static void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu) {
    if (Menu->Page == &MQxChartPage) {
        OLED_ShowMQxPage(OLED, &MQxChartPage.LowerPages[TextMenu.Cursor],
                         &MQSensor[TextMenu.Cursor]);

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

        if (Page == &MonitorPage) {
            OLED_ShowMQxText(OLED, &MonitorPage.LowerPages[i], &MQSensor[i]);

        } else if (Page == &SettingPage) {
            OLED_Printf(OLED, Page->LowerPages[i].X, Page->LowerPages[i].Y,
                        "%s", Page->LowerPages[i].Title);
            OLED_ShowImage(OLED, OLED->Width - 1 - OLED->FontWidth * 6 - 8,
                           Page->LowerPages[i].Y, 8, 8,
                           SettingImage[SettingPage.LowerPages[i].Setting]);

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

void vUpdateTimerCallback(TimerHandle_t pxTimer) {
    if (Menu == &ImageMenu) {
        ImageMenu_Update(Menu, &OLED);
        SelectioneBar_Update(&Bar);

    } else if (Menu == &TextMenu) {
        if (((TextMenu_t *)Menu)->Page != &MQxChartPage) {
            TextMenu_Update(Menu, &OLED);
            SelectioneBar_Update(&Bar);
        }
    }

    for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
        MQSensor_UpdateState(&MQSensor[i]);
    }

    if (StatusLEDSetting->Setting) {
        LED_On(&LED);
    } else {
        LED_Off(&LED);
    }

    if (RestartSetting->Setting) {
        __NVIC_SystemReset();
    }
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            if (Menu == &ImageMenu) {
                if (ImageMenu_CursorDec(Menu)) {
                    SelectioneBar_BindImagePage(
                        &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);
                }

            } else if (Menu == &TextMenu) {
                if (TextMenu.Page == &MQxChartPage) {
                    MQSensor_UpdateThreshold(&MQSensor[TextMenu.Cursor], 128);

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
            if (Menu == &ImageMenu) {
                if (ImageMenu_CursorInc(Menu)) {
                    SelectioneBar_BindImagePage(
                        &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);
                }

            } else if (Menu == &TextMenu) {
                if (TextMenu.Page == &MQxChartPage) {
                    MQSensor_UpdateThreshold(&MQSensor[TextMenu.Cursor], -128);

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
            if (Menu == &ImageMenu) {
                LastMenu = Menu;
                Menu = &TextMenu;
                ImageMenu_EnterLowerPage(&ImageMenu, &TextMenu);
                SelectioneBar_BindTextPage(
                    &Bar,
                    &((TextMenu_t *)Menu)->Page->LowerPages[TextMenu.Cursor]);

            } else if (Menu == &TextMenu) {
                if (TextMenu.Page == &MQxChartPage) {
                    if (TextMenu_CursorInc(Menu)) {
                        SelectioneBar_BindTextPage(
                            &Bar,
                            &((TextMenu_t *)Menu)
                                 ->Page
                                 ->LowerPages[((TextMenu_t *)Menu)->Cursor]);
                    }

                } else if (TextMenu.Page == &SettingPage) {
                    TextPage_ReverseSetting(TextMenu.Page);

                } else {
                    if (TextMenu_EnterLowerPage(Menu)) {
                        LastMenu = Menu;
                        SelectioneBar_BindTextPage(
                            &Bar, &((TextMenu_t *)Menu)
                                       ->Page->LowerPages[TextMenu.Cursor]);
                    }
                }
            }
        }

        if (Key_Read(&KeyCancel)) {
            if (Menu == &ImageMenu) {

            } else if (Menu == &TextMenu) {
                if (LastMenu == &TextMenu) {
                    if (TextMenu_ReturnUpperPage(Menu)) {
                        SelectioneBar_BindTextPage(
                            &Bar, &((TextMenu_t *)Menu)
                                       ->Page->LowerPages[TextMenu.Cursor]);
                    }

                } else if (LastMenu == &ImageMenu) {
                    Menu = &ImageMenu;
                    ImageMenu_ReturnUpperPage(&ImageMenu, &TextMenu);
                    SelectioneBar_BindImagePage(
                        &Bar, &((ImageMenu_t *)Menu)->Page[ImageMenu.Cursor]);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}