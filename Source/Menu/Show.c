#include "main.h"

#define ShowTitleAndTexts(...)                                                 \
    if (TextMenu.Page->TitleY + TextMenu.Page->TitleHeight >= 0) {             \
        OLED_Printf(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,       \
                    TextMenu.Page->Title);                                     \
    }                                                                          \
                                                                               \
    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {             \
        if (TextMenu.Page->LowerPages[i].Y +                                   \
                TextMenu.Page->LowerPages[i].Height <                          \
            0) {                                                               \
            continue;                                                          \
        }                                                                      \
        if (TextMenu.Page->LowerPages[i].Y > OLED.Height - 1) {                \
            break;                                                             \
        }                                                                      \
                                                                               \
        __VA_ARGS__                                                            \
    }

void TextPage_ShowMonitorCallback(void *pvParameters) {
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

void TextPage_ShowMQxCallback(void *pvParameters) {
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

void TextPage_ShowSettingCallback(void *pvParameters) {
    ShowTitleAndTexts(
        OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                    TextMenu.Page->LowerPages[i].Y, "%s",
                    TextMenu.Page->LowerPages[i].Title);

        if (TextMenu.Page->LowerPages[i].ClickCallback ==
            Setting_ReverseCallback) {
            OLED_ShowImage(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
                           TextMenu.Page->LowerPages[i].Y, 8, 8,
                           SettingImage[TextMenu.Page->LowerPages[i].Setting]);
        }

        if (TextMenu.Page->LowerPages[i].ClickCallback ==
            Setting_CursorSwitchIncDecCallback) {
            OLED_Printf(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
                        TextMenu.Page->LowerPages[i].Y, "%d",
                        TextMenu.Page->LowerPages[i].Setting);

            if (TextMenu.Page->LowerPages[i].RotationCallback ==
                Setting_IncDecCallback) {
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

void TextPage_ShowFloatingCallback(void *pvParameters) {
    TextPage_BackCallback(NULL);
    TextMenu.Page->ShowCallback(NULL);
    TextPage_EnterCallback(NULL);

    OLED_ClearBufferArea(&OLED, OLED.Width / 8, OLED.Height / 8,
                         OLED.Width - OLED.Width / 4,
                         OLED.Height - OLED.Height / 4);

    OLED_DrawHollowRectangle(&OLED, TextMenu.Page->TitleX,
                             TextMenu.Page->TitleY, TextMenu.Page->TitleWidth,
                             TextMenu.Page->TitleHeight);

    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {
        if (TextMenu.Page->LowerPages[i].X +
                    TextMenu.Page->LowerPages[i].Width >
                TextMenu.Page->TitleX + TextMenu.Page->TitleWidth ||
            TextMenu.Page->LowerPages[i].Y +
                    TextMenu.Page->LowerPages[i].Height >
                TextMenu.Page->TitleY + TextMenu.Page->TitleHeight) {
            continue;
        }
        OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                    TextMenu.Page->LowerPages[i].Y,
                    TextMenu.Page->LowerPages[i].Title);

        if (i == TextMenu.Page->NumOfLowerPages - 1) {
            OLED_Printf(&OLED, OLED.Width / 2 - 7 / 2 * OLED.FontWidth,
                        TextMenu.Page->TitleY + OLED.FontHeight, "Success");
        }
    }
}

void ImagePage_ShowCallback(void *pvParameters) {
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