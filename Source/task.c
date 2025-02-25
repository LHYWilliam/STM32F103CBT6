#include "main.h"

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    if (Menu == &TextMenu) {
        TextMenu.Page->ShowCallback(NULL);

    } else if (Menu == &ImageMenu) {
        ImageMenu.ShowCallback(NULL);
    }

    OLED_ShowSelectioneBar(&OLED, &Bar);

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);
}

void vUpdateTimerCallback(TimerHandle_t pxTimer) {
    if (Menu == &TextMenu) {
        TextMenu.Page->UpdateCallback(NULL);

    } else if (Menu == &ImageMenu) {
        ImageMenu_Update(Menu, &OLED);
    }

    SelectioneBar_Update(&Bar);

    for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
        MQSensor_UpdateState(&MQSensor[i]);
    }

    if (LEDSetting->Setting) {
        LED_On(&LED);
    } else {
        LED_Off(&LED);
    }
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        int16_t Encode = Encoder_GetCount(&Encoder);

        if (Encode <= -3 || Encode >= 3) {
            if (Menu == &ImageMenu) {
                ImageMenu.RotationCallback(Encode);

            } else if (Menu == &TextMenu) {
                TextMenu.Page->LowerPages[TextMenu.Cursor].RotationCallback(
                    Encode);
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu == &ImageMenu) {
                ImageMenu.ClickCallback(NULL);

            } else if (Menu == &TextMenu) {
                TextMenu.Page->LowerPages[TextMenu.Cursor].ClickCallback(NULL);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Update(int16_t Y) {
    PositionUpdate(TextMenu.Page->TitleY, Y);

    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {
        if (i == 0) {
            Y = Y + TextMenu.Page->TitleHeight / 4 -
                TextMenu.Page->LowerPages[0].Height / 2 + 1;

        } else if (i == 1) {
            Y = TextMenu.Page->TitleY + TextMenu.Page->TitleHeight + 1;

        } else {
            Y = TextMenu.Page->LowerPages[i - 1].Y +
                TextMenu.Page->LowerPages[i - 1].Height + 2;
        }

        PositionUpdate(TextMenu.Page->LowerPages[i].Y, Y);
    }
}

void TextPageUpdateOneByOneCallback(void *pvParameters) {
    TextMenu.PageNumber = TextMenu_PageNumber(TextMenu);

    int16_t Y = TextMenu.Page->TitleY;

    if (TextMenu.Cursor == 0) {
        Y = 0;

    } else if (TextMenu.Page->LowerPages[TextMenu.Cursor].Y < 1) {
        Y = TextMenu.Page->TitleY -
            TextMenu.Page->LowerPages[TextMenu.Cursor].Y + 1;

    } else if (TextMenu.Page->LowerPages[TextMenu.Cursor].Y +
                   TextMenu.Page->LowerPages[TextMenu.Cursor].Height >=
               OLED.Height) {
        Y = TextMenu.Page->TitleY -
            (TextMenu.Page->LowerPages[TextMenu.Cursor].Y - OLED.Height +
             TextMenu.Page->LowerPages[TextMenu.Cursor].Height) -
            1;
    }

    Update(Y);
}

void TextPageUpdatePageByPageCallback(void *pvParameters) {
    TextMenu.PageNumber = TextMenu_PageNumber(TextMenu);

    int16_t Y = TextMenu.Page->TitleY;

    if (TextMenu.PageNumber == 0) {
        Y = 0;

    } else {
        uint8_t Index =
            TextMenu.TextCountOfHomePage +
            TextMenu.TextCountOfOtherPage * (TextMenu.PageNumber - 1);
        Y = TextMenu.Page->TitleY - TextMenu.Page->LowerPages[Index].Y + 1;
    }

    Update(Y);
}

#define ShowTitleAndTexts(...)                                                 \
    if (TextMenu.Page->TitleY + TextMenu.Page->TitleHeight >= 0) {             \
        OLED_Printf(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,       \
                    TextMenu.Page->Title);                                     \
    }                                                                          \
                                                                               \
    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {             \
        if (TextMenu.Page->LowerPages[i].Y < 0) {                              \
            continue;                                                          \
        }                                                                      \
        if (TextMenu.Page->LowerPages[i].Y +                                   \
                TextMenu.Page->LowerPages[i].Height >=                         \
            OLED.Height) {                                                     \
            break;                                                             \
        }                                                                      \
                                                                               \
        __VA_ARGS__                                                            \
    }

void ShowMonitorPageCallback(void *pvParameters) {
    ShowTitleAndTexts(
        if (i == 0) {
            OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                        TextMenu.Page->LowerPages[i].Y, "%s",
                        TextMenu.Page->LowerPages[i].Title);
        } else {
            OLED_Printf(&OLED, MonitorPage.LowerPages[i].X,
                        MonitorPage.LowerPages[i].Y, "%-6s",
                        MonitorPage.LowerPages[i].Title);
            OLED_Printf(
                &OLED, OLED.Width - 1 - OLED.FontWidth * 12,
                MonitorPage.LowerPages[i].Y, "%.3f %6s",
                ADCToVoltage(MQSensor[i - 1].Data[MQSensor[i - 1].Index]),
                MQSensor[i - 1].State ? "Danger" : "Safe");
        });
}

void ShowMQxPageCallback(void *pvParameters) {
    OLED_Printf(&OLED, TextMenu.Page->LowerPages[0].X,
                TextMenu.Page->LowerPages[0].Y + 1, "%s",
                TextMenu.Page->LowerPages[0].Title);

    OLED_Printf(&OLED, 16, 1, "%s %s", TextMenu.Page->Title,
                MQSensor[TextMenu.Page->UpperPage->Cursor - 1].State ? "Danger"
                                                                     : "Safe");

    OLED_Printf(
        &OLED, 0, OLED.Height - OLED.FontHeight - 1, "%.3f V",
        ADCToVoltage(
            MQSensor[TextMenu.Page->UpperPage->Cursor - 1]
                .Data[MQSensor[TextMenu.Page->UpperPage->Cursor - 1].Index]));

    OLED_ShowChart(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,
                   TextMenu.Page->TitleWidth, TextMenu.Page->TitleHeight,
                   MQSensor[TextMenu.Page->UpperPage->Cursor - 1].Data,
                   MQSensor[TextMenu.Page->UpperPage->Cursor - 1].Length,
                   MQSensor[TextMenu.Page->UpperPage->Cursor - 1].Index);
    OLED_DrawHLine(
        &OLED, TextMenu.Page->TitleX,
        OLED_ADCToY(MQSensor[TextMenu.Page->UpperPage->Cursor - 1].Threshold,
                    TextMenu.Page->TitleY, TextMenu.Page->TitleHeight),
        TextMenu.Page->TitleWidth, 2);
}

void ShowSettingPageCallback(void *pvParameters) {
    ShowTitleAndTexts(
        OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                    TextMenu.Page->LowerPages[i].Y, "%s",
                    TextMenu.Page->LowerPages[i].Title);

        if (TextMenu.Page->LowerPages[i].ClickCallback ==
            SettingReverseCallback) {
            OLED_ShowImage(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
                           TextMenu.Page->LowerPages[i].Y, 8, 8,
                           SettingImage[TextMenu.Page->LowerPages[i].Setting]);
        }

        if (TextMenu.Page->LowerPages[i].ClickCallback ==
            SettingCursorToIncDecCallback) {
            OLED_Printf(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
                        TextMenu.Page->LowerPages[i].Y, "%d",
                        TextMenu.Page->LowerPages[i].Setting);

            if (TextMenu.Page->LowerPages[i].RotationCallback ==
                SettingIncDecCallback) {
                uint8_t number;
                if (TextMenu.Page->LowerPages[i].Setting == 0) {
                    number = 1;
                } else {
                    number = (uint8_t)log10(
                                 abs(TextMenu.Page->LowerPages[i].Setting)) +
                             1;
                }
                number += TextMenu.Page->LowerPages[i].Setting < 0 ? 1 : 0;

                OLED_DrawHLine(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
                               TextMenu.Page->LowerPages[i].Y + OLED.FontHeight,
                               OLED.FontWidth * number, 1);
            }
        });
}

void ShowFloatPageCallback(void *pvParameters) {
    BackTextPageCallback(NULL);
    TextMenu.Page->ShowCallback(NULL);
    EnterTextPageCallback(NULL);

    OLED_ClearBufferArea(&OLED, OLED.Width / 8, OLED.Height / 8,
                         OLED.Width - OLED.Width / 4,
                         OLED.Height - OLED.Height / 4);

    OLED_DrawHollowRectangle(&OLED, OLED.Width / 8, OLED.Height / 8,
                             OLED.Width - OLED.Width / 4,
                             OLED.Height - OLED.Height / 4);

    OLED_Printf(&OLED, OLED.Width / 8 + 2, OLED.Height / 8 + 2,
                TextMenu.Page->LowerPages[1].Title);
}

void ShowImageMenuCallback(void *pvParameters) {
    for (uint8_t i = 0; i < ImageMenu.NumOfPages; i++) {
        if (ImageMenu.Page[i].ImageX + ImageMenu.ImageWidth < 0) {
            continue;
        }
        if (ImageMenu.Page[i].ImageX >= OLED.Width) {
            break;
        }

        OLED_ShowImage(&OLED, ImageMenu.Page[i].ImageX,
                       ImageMenu.Page[i].ImageY, ImageMenu.ImageWidth,
                       ImageMenu.ImageHeight, ImageMenu.Page[i].Image);

        OLED_Printf(&OLED, ImageMenu.Page[i].TitleX, ImageMenu.Page[i].TitleY,
                    "%s", ImageMenu.Page[i].Title);
    }
}

void BackHomePageCallbck(void *pvParameters) {
    Menu = &ImageMenu;
    if (ImageMenu_ReturnUpperPage(&ImageMenu, &TextMenu)) {
        SelectioneBar_BindImagePage(&Bar, &ImageMenu.Page[ImageMenu.Cursor]);
    }
}

void ImagePageEnterTextPageCallback(void *pvParameters) {
    Menu = &TextMenu;
    ImageMenu_EnterLowerPage(&ImageMenu, &TextMenu);
    SelectioneBar_BindTextPage(&Bar,
                               &TextMenu.Page->LowerPages[TextMenu.Cursor]);
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

void RestartSettingCallback(void *pvParameters) { __NVIC_SystemReset(); }

void SettingReverseCallback(void *pvParameters) {
    TextPage_ReverseSetting(TextMenu.Page);
}

void SettingIncDecCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        TextMenu.Page->LowerPages[TextMenu.Cursor].Setting++;

    } else if (Encoder <= -3) {
        TextMenu.Page->LowerPages[TextMenu.Cursor].Setting--;
    }
}

void SettingCursorToIncDecCallback(void *pvParameters) {
    if (TextMenu.Page->LowerPages[TextMenu.Cursor].RotationCallback ==
        TextMenuCursorCallback) {
        TextMenu.Page->LowerPages[TextMenu.Cursor].RotationCallback =
            SettingIncDecCallback;

    } else if (TextMenu.Page->LowerPages[TextMenu.Cursor].RotationCallback ==
               SettingIncDecCallback) {
        TextMenu.Page->LowerPages[TextMenu.Cursor].RotationCallback =
            TextMenuCursorCallback;
    }
}

void SettingSaveCallback(void *pvParameters) {
    uint8_t Setting[32];
    for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
        Setting[i - 1] = SettingPage.LowerPages[i].Setting;
    }

    W25Q64_SectorErase(&W25Q64, 0);
    W25Q64_PageProgram(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

    EnterTextPageCallback(NULL);
}

void SettingLoad(void *pvParameters) {
    uint8_t Setting[32];
    W25Q64_ReadData(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

    for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
        SettingPage.LowerPages[i].Setting = Setting[i - 1];
    }
}

void SettingLoadCallback(void *pvParameters) {
    uint8_t Setting[32];
    W25Q64_ReadData(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

    for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
        SettingPage.LowerPages[i].Setting = Setting[i - 1];
    }

    EnterTextPageCallback(NULL);
}