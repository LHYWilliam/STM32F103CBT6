#include <string.h>

#include "Menu.h"
#include "OLED.h"

void EmptyCallbackPtr(void *pvParameters) {}
void EmptyCallbackInt(int16_t pvParameter) {}

void TextPage_Init(TextPage_t *self, OLED_t *OLED) {
    if (IsChinese(self->Title)) {
        OLEDFont Font = OLED->Font;
        OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        if (!self->TitleX) {
            self->TitleX = OLED->Width / 2 - strlen(self->Title) /
                                                 OLED_ChineseBytesCount *
                                                 OLED->FontWidth / 2;
        }
        if (!self->TitleWidth) {
            self->TitleWidth =
                strlen(self->Title) / OLED_ChineseBytesCount * OLED->FontWidth;
        }
        if (!self->TitleHeight) {
            self->TitleHeight = OLED->FontHeight * 2;
        }
        OLED_SetFont(OLED, Font);

    } else {
        if (!self->TitleX) {
            self->TitleX =
                OLED->Width / 2 - strlen(self->Title) * OLED->FontWidth / 2;
        }
        if (!self->TitleWidth) {
            self->TitleWidth = strlen(self->Title) * OLED->FontWidth;
        }
        if (!self->TitleHeight) {
            self->TitleHeight = OLED->FontHeight * 2;
        }
    }

    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        if (IsChinese(self->LowerPages[i].Title)) {
            OLEDFont Font = OLED->Font;
            OLED_SetFont(OLED, OLEDFont_Chinese12X12);
            self->LowerPages[i].Width = strlen(self->LowerPages[i].Title) /
                                        OLED_ChineseBytesCount *
                                        OLED->FontWidth;
            self->LowerPages[i].Height = OLED->FontHeight;
            OLED_SetFont(OLED, Font);

        } else {
            self->LowerPages[i].Width =
                strlen(self->LowerPages[i].Title) * OLED->FontWidth;
            self->LowerPages[i].Height = OLED->FontHeight;
        }

        if (i == 0) {
            self->LowerPages[i].UpperPage = self->UpperPage;

        } else {
            self->LowerPages[i].UpperPage = self;
        }

        if (!self->LowerPages[i].UpdateCallback) {
            self->LowerPages[i].UpdateCallback = EmptyCallbackPtr;
        }
        if (!self->LowerPages[i].ShowCallback) {
            self->LowerPages[i].ShowCallback = EmptyCallbackPtr;
        }
        if (!self->LowerPages[i].ClickCallback) {
            self->LowerPages[i].ClickCallback = EmptyCallbackPtr;
        }
        if (!self->LowerPages[i].RotationCallback) {
            self->LowerPages[i].RotationCallback = EmptyCallbackInt;
        }

        TextPage_Init(&self->LowerPages[i], OLED);
    }
}

void TextPage_ResetSetY(TextPage_t *self) {
    for (uint8_t i = self->Cursor; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].Y = 0;
    }
    for (int8_t i = self->Cursor - 1; i >= 0; i--) {
        self->LowerPages[i].Y =
            self->LowerPages[i + 1].Y - self->LowerPages[i].Height - 2;
    }
}

void TextPage_ReverseSetting(TextPage_t *self) {
    self->LowerPages[self->Cursor].Setting =
        !self->LowerPages[self->Cursor].Setting;
}

void TextMenu_Init(TextMenu_t *self, OLED_t *OLED) {
    if (self->Page) {
        TextPage_Init(self->Page, OLED);
    }
}

ErrorStatus TextMenu_CursorInc(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages >= 2) {
        self->Page->Cursor =
            (self->Page->Cursor + 1) % self->Page->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_CursorDec(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages >= 2) {
        self->Page->Cursor =
            (self->Page->Cursor + self->Page->NumOfLowerPages - 1) %
            self->Page->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_EnterLowerPage(TextMenu_t *self) {
    if (self->Page->LowerPages[self->Page->Cursor].NumOfLowerPages) {
        self->Page = &self->Page->LowerPages[self->Page->Cursor];

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_ReturnUpperPage(TextMenu_t *self) {
    if (self->Page->UpperPage) {
        self->Page = self->Page->UpperPage;

        return SUCCESS;
    }

    return ERROR;
}

void ImagePage_Init(ImagePage_t *self, OLED_t *OLED) {
    if (!self->ImageY) {
        self->ImageY = OLED->Height / 2 - self->ImageHeight / 2;
    }

    if (IsChinese(self->Title)) {
        OLEDFont Font = OLED->Font;
        OLED_SetFont(OLED, OLEDFont_Chinese12X12);
        self->TitleY = self->ImageY + self->ImageHeight + OLED->FontHeight;
        self->TitleWidth =
            strlen(self->Title) / OLED_ChineseBytesCount * OLED->FontWidth;
        self->TitleHeight = OLED->FontHeight;
        OLED_SetFont(OLED, Font);

    } else {
        self->TitleY = self->ImageY + self->ImageHeight + OLED->FontHeight;
        self->TitleWidth = strlen(self->Title) * OLED->FontWidth;
        self->TitleHeight = OLED->FontHeight;
    }
}

void ImageMenu_Init(ImageMenu_t *self, OLED_t *OLED) {
    for (uint8_t i = 0; i < self->NumOfPages; i++) {
        if (!self->Page[i].ImageWidth) {
            self->Page[i].ImageWidth = self->ImageWidth;
        }
        if (!self->Page[i].ImageHeight) {
            self->Page[i].ImageHeight = self->ImageHeight;
        }

        ImagePage_Init(&self->Page[i], OLED);
    }

    if (!self->ShowCallback) {
        self->ShowCallback = EmptyCallbackPtr;
    }
    if (!self->ClickCallback) {
        self->ShowCallback = EmptyCallbackPtr;
    }
    if (!self->RotationCallback) {
        self->RotationCallback = EmptyCallbackInt;
    }
}

void ImageMenu_Update(ImageMenu_t *self, OLED_t *OLED) {
    int16_t X =
        self->Page[0].ImageX - (self->Page[self->Cursor].ImageX +
                                self->Page[0].ImageWidth / 2 - OLED->Width / 2);
    PositionUpdate(self->Page[0].ImageX, X);
    self->Page[0].TitleX = self->Page[0].ImageX + self->Page[0].ImageWidth / 2 -
                           self->Page[0].TitleWidth / 2;

    for (uint8_t i = 1; i < self->NumOfPages; i++) {
        PositionUpdate(self->Page[i].ImageX, self->Page[i - 1].ImageX +
                                                 self->Page[i].ImageWidth +
                                                 self->Space);

        self->Page[i].TitleX = self->Page[i].ImageX +
                               self->Page[i].ImageWidth / 2 -
                               self->Page[i].TitleWidth / 2;
    }
}

ErrorStatus ImageMenu_CursorInc(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->NumOfPages;

    return SUCCESS;
}

ErrorStatus ImageMenu_CursorDec(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + self->NumOfPages - 1) % self->NumOfPages;

    return SUCCESS;
}

ErrorStatus ImageMenu_EnterLowerPage(ImageMenu_t *self, TextMenu_t *TextMenu) {
    TextMenu->Page = self->Page[self->Cursor].TextPage;

    return SUCCESS;
}

ErrorStatus ImageMenu_ReturnUpperPage(ImageMenu_t *self, TextMenu_t *TextMenu) {
    TextPage_ResetSetY(TextMenu->Page);

    return SUCCESS;
}

void SelectioneBar_BindTextPage(SelectioneBar_t *self, TextPage_t *Page) {
    self->TargetX = &Page->X;
    self->TargetY = &Page->Y;
    self->TargetWidth = &Page->Width;
    self->TargetHeight = &Page->Height;
}

void SelectioneBar_BindImagePage(SelectioneBar_t *self, ImagePage_t *Page) {
    self->TargetX = &Page->TitleX;
    self->TargetY = &Page->TitleY;
    self->TargetWidth = &Page->TitleWidth;
    self->TargetHeight = &Page->TitleHeight;
}

void SelectioneBar_Update(SelectioneBar_t *self) {
    PositionUpdate(self->X, *self->TargetX - 1);
    PositionUpdate(self->Y, *self->TargetY - 1);
    PositionUpdate(self->Width, *self->TargetWidth + 2);
    PositionUpdate(self->Height, *self->TargetHeight + 2);
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar) {
    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y,
                     SelectioneBar->Width, SelectioneBar->Height);
}