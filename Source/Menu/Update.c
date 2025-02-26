#include "main.h"

void TextPage_UpdateCallback(void *pvParameters) {
    int16_t Y = TextMenu.Page->TitleY;

    if (TextMenu.Page->Cursor == 0 || TextMenu.Page->Cursor == 1) {
        Y = 0;

    } else if (TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Y < 0) {
        Y = TextMenu.Page->TitleY -
            TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Y;

    } else if (TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Y +
                   TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Height >
               OLED.Height - 1) {
        Y = TextMenu.Page->TitleY -
            (TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Y - OLED.Height +
             TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Height) -
            1;
    }

    PositionUpdate(TextMenu.Page->TitleY, Y, 1);

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

        PositionUpdate(TextMenu.Page->LowerPages[i].Y, Y, 1);
    }
}

void TextPage_UpdateFloatCallback(void *pvParameters) {
    PositionUpdate(TextMenu.Page->TitleX, OLED.Width / 8, 1);
    PositionUpdate(TextMenu.Page->TitleY, OLED.Height / 8, 1);
    PositionUpdate(TextMenu.Page->TitleWidth, OLED.Width - OLED.Width / 4 - 1,
                   2); // -1 for border
    PositionUpdate(TextMenu.Page->TitleHeight,
                   OLED.Height - OLED.Height / 4 - 1, 2); // -1 for border

    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {
        PositionUpdate(
            TextMenu.Page->LowerPages[i].X,
            OLED.Width / 2 - TextMenu.Page->LowerPages[i].TitleWidth / 2, 1);
        PositionUpdate(TextMenu.Page->LowerPages[i].Y,
                       TextMenu.Page->TitleY + TextMenu.Page->TitleHeight -
                           OLED.FontHeight * 2,
                       1);
    }
}