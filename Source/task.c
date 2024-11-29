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

    if (TextMenu.Page->UpperPage == &MonitorPage) {
        OLED_ShowMQxPage(OLED, &MQxChartPage[TextMenu.Page->UpperPage->Cursor],
                         &MQSensor[TextMenu.Page->UpperPage->Cursor - 1]);

    } else {
        OLED_ShowTextPage(OLED, Menu->Page);
    }

    OLED_ShowSelectioneBar(OLED, &Bar);
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

        if (Page == &MonitorPage && i != 0) {
            OLED_ShowMQxText(OLED, &MonitorPage.LowerPages[i],
                             &MQSensor[i - 1]);

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
    OLED_Printf(OLED, MQxPage->LowerPages[0].X, MQxPage->LowerPages[0].Y + 1,
                "%s", MQxPage->LowerPages[0].Title);

    OLED_Printf(OLED, 16, 1, "%s %s", MQxPage->Title,
                MQSensor->State ? "Danger" : "Safe");

    OLED_Printf(OLED, 0, OLED->Height - OLED->FontHeight - 1, "%.3f V",
                ADCToVoltage(MQSensor->Data[MQSensor->Index]));

    OLED_ShowChart(OLED, MQxPage->TitleX, MQxPage->TitleY, MQxPage->TitleWidth,
                   MQxPage->TitleHeight, MQSensor->Data, MQSensor->Length,
                   MQSensor->Index);
    OLED_DrawHLine(
        OLED, MQxPage->TitleX,
        OLED_ADCToY(MQSensor->Threshold, MQxPage->TitleY, MQxPage->TitleHeight),
        MQxPage->TitleWidth, 2);
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

    } else if (Menu == &TextMenu) {
        if (TextMenu.Page->UpperPage != &MonitorPage) {
            TextMenu_Update(Menu, &OLED);
        }
    }

    SelectioneBar_Update(&Bar);

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
        int16_t Encode = Encoder_Get(&Encoder);

        if (Encode <= -3 || Encode >= 3) {
            if (Menu == &ImageMenu) {
                if (ImageMenu.RotationCallback) {
                    ImageMenu.RotationCallback(Encode);
                }

            } else if (Menu == &TextMenu) {
                if (TextMenu.Page->RotationCallback) {
                    TextMenu.Page->RotationCallback(Encode);
                }
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu == &ImageMenu) {
                if (ImageMenu.ClickCallback) {
                    ImageMenu.ClickCallback(NULL);
                }

            } else if (Menu == &TextMenu) {
                if (TextMenu.Page->LowerPages[TextMenu.Cursor].ClickCallback) {
                    TextMenu.Page->LowerPages[TextMenu.Cursor].ClickCallback(
                        NULL);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void BackHomePageCallbck(void *pvParameters) {
    Menu = &ImageMenu;
    if (ImageMenu_ReturnUpperPage(&ImageMenu, &TextMenu)) {
        SelectioneBar_BindImagePage(&Bar, &ImageMenu.Page[ImageMenu.Cursor]);
    }
}

void EnterTextPageCallback(void *pvParameters) {
    if (TextMenu_EnterLowerPage(&TextMenu)) {
        SelectioneBar_BindTextPage(&Bar,
                                   &TextMenu.Page->LowerPages[TextMenu.Cursor]);
    }
}

void BackTextPageCallback(void *pvParameters) {
    if (TextMenu_ReturnUpperPage(&TextMenu)) {
        SelectioneBar_BindTextPage(&Bar,
                                   &TextMenu.Page->LowerPages[TextMenu.Cursor]);
    }
}

void SettingCallback(void *pvParameters) {
    TextPage_ReverseSetting(TextMenu.Page);
}

void ThresholdCallback(int16_t Encoder) {
    MQSensor_UpdateThreshold(&MQSensor[TextMenu.Page->UpperPage->Cursor - 1],
                             Encoder > 0 ? -128 : +128);
}

void TextMenuCursorCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        if (TextMenu_CursorInc(Menu)) {
            SelectioneBar_BindTextPage(
                &Bar, &((TextMenu_t *)Menu)
                           ->Page->LowerPages[((TextMenu_t *)Menu)->Cursor]);
        }

    } else if (Encoder <= -3) {
        if (TextMenu_CursorDec(Menu)) {
            SelectioneBar_BindTextPage(
                &Bar, &((TextMenu_t *)Menu)
                           ->Page->LowerPages[((TextMenu_t *)Menu)->Cursor]);
        }
    }
}

void ImageMenuCursorCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        if (ImageMenu_CursorInc(Menu)) {
            SelectioneBar_BindImagePage(&Bar,
                                        &ImageMenu.Page[ImageMenu.Cursor]);
        }

    } else if (Encoder <= -3) {
        if (ImageMenu_CursorDec(Menu)) {
            SelectioneBar_BindImagePage(&Bar,
                                        &ImageMenu.Page[ImageMenu.Cursor]);
        }
    }
}

void ImagePageEnterTextPageCallback(void *pvParameters) {
    Menu = &TextMenu;
    ImageMenu_EnterLowerPage(&ImageMenu, &TextMenu);
    SelectioneBar_BindTextPage(&Bar,
                               &TextMenu.Page->LowerPages[TextMenu.Cursor]);
}