#include <string.h>

#include "Menu.h"
#include "OLED.h"

void EmptyCallbackPtr(void *pvParameters) {}
void EmptyCallbackInt(int16_t pvParameter) {}

void TextPage_Init(TextPage_t *Self, OLED_t *OLED) {
    if (IsChinese(Self->Title)) {
        OLEDFont Font = OLED->Font;
        OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        if (!Self->TitleX) {
            Self->TitleX = OLED->Width / 2 - strlen(Self->Title) /
                                                 OLED_ChineseBytesCount *
                                                 OLED->FontWidth / 2;
        }
        if (!Self->TitleWidth) {
            Self->TitleWidth =
                strlen(Self->Title) / OLED_ChineseBytesCount * OLED->FontWidth;
        }
        if (!Self->TitleHeight) {
            Self->TitleHeight = OLED->FontHeight * 2;
        }
        OLED_SetFont(OLED, Font);

    } else {
        if (!Self->TitleX) {
            Self->TitleX =
                OLED->Width / 2 - strlen(Self->Title) * OLED->FontWidth / 2;
        }
        if (!Self->TitleWidth) {
            Self->TitleWidth = strlen(Self->Title) * OLED->FontWidth;
        }
        if (!Self->TitleHeight) {
            Self->TitleHeight = OLED->FontHeight * 2;
        }
    }

    for (uint8_t i = 0; i < Self->NumOfLowerPages; i++) {
        if (IsChinese(Self->LowerPages[i].Title)) {
            OLEDFont Font = OLED->Font;
            OLED_SetFont(OLED, OLEDFont_Chinese12X12);
            Self->LowerPages[i].Width = strlen(Self->LowerPages[i].Title) /
                                        OLED_ChineseBytesCount *
                                        OLED->FontWidth;
            Self->LowerPages[i].Height = OLED->FontHeight;
            OLED_SetFont(OLED, Font);

        } else {
            Self->LowerPages[i].Width =
                strlen(Self->LowerPages[i].Title) * OLED->FontWidth;
            Self->LowerPages[i].Height = OLED->FontHeight;
        }

        if (i == 0) {
            Self->LowerPages[i].UpperPage = Self->UpperPage;

        } else {
            Self->LowerPages[i].UpperPage = Self;
        }

        if (!Self->LowerPages[i].UpdateCallback) {
            Self->LowerPages[i].UpdateCallback = EmptyCallbackPtr;
        }
        if (!Self->LowerPages[i].ShowCallback) {
            Self->LowerPages[i].ShowCallback = EmptyCallbackPtr;
        }
        if (!Self->LowerPages[i].ClickCallback) {
            Self->LowerPages[i].ClickCallback = EmptyCallbackPtr;
        }
        if (!Self->LowerPages[i].RotationCallback) {
            Self->LowerPages[i].RotationCallback = EmptyCallbackInt;
        }

        TextPage_Init(&Self->LowerPages[i], OLED);
    }
}

void TextPage_ResetSetY(TextPage_t *Self) {
    for (uint8_t i = Self->Cursor; i < Self->NumOfLowerPages; i++) {
        Self->LowerPages[i].Y = 0;
    }
    for (int8_t i = Self->Cursor - 1; i >= 0; i--) {
        Self->LowerPages[i].Y =
            Self->LowerPages[i + 1].Y - Self->LowerPages[i].Height - 2;
    }
}

void TextPage_ReverseSetting(TextPage_t *Self) {
    Self->Setting = !Self->Setting;
}

void TextMenu_Init(TextMenu_t *Self, OLED_t *OLED) {
    if (Self->Page) {
        TextPage_Init(Self->Page, OLED);
    }
}

ErrorStatus TextPage_CursorInc(TextPage_t *Self) {
    if (Self->NumOfLowerPages >= 2) {
        Self->Cursor = (Self->Cursor + 1) % Self->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_CursorDec(TextPage_t *Self) {
    if (Self->NumOfLowerPages >= 2) {
        Self->Cursor =
            (Self->Cursor + Self->NumOfLowerPages - 1) % Self->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_EnterLowerPage(TextPage_t **Self) {
    if ((*Self)->LowerPages[(*Self)->Cursor].NumOfLowerPages) {
        *Self = &(*Self)->LowerPages[(*Self)->Cursor];

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_ReturnUpperPage(TextPage_t **Self) {
    if ((*Self)->UpperPage) {
        *Self = (*Self)->UpperPage;

        return SUCCESS;
    }

    return ERROR;
}

void ImagePage_Init(ImagePage_t *Self, OLED_t *OLED) {
    if (!Self->ImageY) {
        Self->ImageY = OLED->Height / 2 - Self->ImageHeight / 2;
    }

    if (IsChinese(Self->Title)) {
        OLEDFont Font = OLED->Font;
        OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        Self->TitleY = Self->ImageY + Self->ImageHeight + OLED->FontHeight;
        Self->TitleWidth =
            strlen(Self->Title) / OLED_ChineseBytesCount * OLED->FontWidth;
        Self->TitleHeight = OLED->FontHeight;
        OLED_SetFont(OLED, Font);

    } else {
        Self->TitleY      = Self->ImageY + Self->ImageHeight + OLED->FontHeight;
        Self->TitleWidth  = strlen(Self->Title) * OLED->FontWidth;
        Self->TitleHeight = OLED->FontHeight;
    }
}

void ImageMenu_Init(ImageMenu_t *Self, OLED_t *OLED) {
    for (uint8_t i = 0; i < Self->NumOfPages; i++) {
        if (!Self->Page[i].ImageWidth) {
            Self->Page[i].ImageWidth = Self->ImageWidth;
        }
        if (!Self->Page[i].ImageHeight) {
            Self->Page[i].ImageHeight = Self->ImageHeight;
        }

        ImagePage_Init(&Self->Page[i], OLED);
    }

    if (!Self->ShowCallback) {
        Self->ShowCallback = EmptyCallbackPtr;
    }
    if (!Self->ClickCallback) {
        Self->ShowCallback = EmptyCallbackPtr;
    }
    if (!Self->RotationCallback) {
        Self->RotationCallback = EmptyCallbackInt;
    }
}

void ImageMenu_Update(ImageMenu_t *Self, OLED_t *OLED) {
    int16_t X =
        Self->Page[0].ImageX - (Self->Page[Self->Cursor].ImageX +
                                Self->Page[0].ImageWidth / 2 - OLED->Width / 2);
    PositionUpdate(Self->Page[0].ImageX, X, 1);
    Self->Page[0].TitleX = Self->Page[0].ImageX + Self->Page[0].ImageWidth / 2 -
                           Self->Page[0].TitleWidth / 2;

    for (uint8_t i = 1; i < Self->NumOfPages; i++) {
        PositionUpdate(Self->Page[i].ImageX,
                       Self->Page[i - 1].ImageX + Self->Page[i].ImageWidth +
                           Self->Space,
                       1);

        Self->Page[i].TitleX = Self->Page[i].ImageX +
                               Self->Page[i].ImageWidth / 2 -
                               Self->Page[i].TitleWidth / 2;
    }
}

ErrorStatus ImageMenu_CursorInc(ImageMenu_t *Self) {
    Self->Cursor = (Self->Cursor + 1) % Self->NumOfPages;

    return SUCCESS;
}

ErrorStatus ImageMenu_CursorDec(ImageMenu_t *Self) {
    Self->Cursor = (Self->Cursor + Self->NumOfPages - 1) % Self->NumOfPages;

    return SUCCESS;
}

ErrorStatus ImageMenu_EnterLowerPage(ImageMenu_t *Self, TextMenu_t *TextMenu) {
    TextMenu->Page = Self->Page[Self->Cursor].TextPage;

    return SUCCESS;
}

ErrorStatus ImageMenu_ReturnUpperPage(ImageMenu_t *Self, TextMenu_t *TextMenu) {
    TextPage_ResetSetY(TextMenu->Page);

    return SUCCESS;
}

void SelectioneBar_BindTextPage(SelectioneBar_t *Self, TextPage_t *Page) {
    Self->TargetX      = &Page->X;
    Self->TargetY      = &Page->Y;
    Self->TargetWidth  = &Page->Width;
    Self->TargetHeight = &Page->Height;
}

void SelectioneBar_BindImagePage(SelectioneBar_t *Self, ImagePage_t *Page) {
    Self->TargetX      = &Page->TitleX;
    Self->TargetY      = &Page->TitleY;
    Self->TargetWidth  = &Page->TitleWidth;
    Self->TargetHeight = &Page->TitleHeight;
}

void SelectioneBar_Update(SelectioneBar_t *Self) {
    PositionUpdate(Self->X, *Self->TargetX - 1, 1);
    PositionUpdate(Self->Y, *Self->TargetY - 1, 1);
    PositionUpdate(Self->Width, *Self->TargetWidth + 2, 1);
    PositionUpdate(Self->Height, *Self->TargetHeight + 2, 1);
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar) {
    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y,
                     SelectioneBar->Width, SelectioneBar->Height);
}