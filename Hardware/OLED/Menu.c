#include <string.h>

#include "Menu.h"
#include "OLED.h"

void TextPage_Init(TextPage_t *self, OLED_t *OLED, TextMenu_t *Menu) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        if (IsChinese(self->Title)) {
            OLEDFont Font = OLED->Font;
            OLED_SetFont(OLED, OLEDFont_Chinese12X12);
            if (!self->TitleX) {
                self->TitleX = OLED->Width / 2 - strlen(self->Title) /
                                                     OLED_ChineseBytesCount *
                                                     OLED->FontWidth / 2;
            }
            if (!self->TitleWidth) {
                self->TitleWidth = strlen(self->Title) /
                                   OLED_ChineseBytesCount * OLED->FontWidth;
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

        self->LowerPages[i].X += 1;
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
            PositionUpdate(self->LowerPages[0].Y,
                           self->TitleY + self->TitleHeight + 1);
        } else {
            PositionUpdate(self->LowerPages[i].Y,
                           self->LowerPages[i - 1].Y +
                               self->LowerPages[i - 1].Height + 2);
        }

        self->LowerPages[i].UpperPage = self;

        TextPage_Init(&self->LowerPages[i], OLED, Menu);
    }
}

void TextPage_SetY(TextPage_t *self, int16_t Y) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].Y = Y;
    }
}

void TextPage_ReverseSetting(TextPage_t *self) {
    self->LowerPages[self->Cursor].Setting =
        !self->LowerPages[self->Cursor].Setting;
}

void TextMenu_Init(TextMenu_t *self, OLED_t *OLED) {
    TextPage_Init(self->Page, OLED, self);
}

void TextMenu_Update(TextMenu_t *self, OLED_t *OLED) {
    self->PageNumber = TextMenu_PageNumber(self);

    int16_t Y = self->Page->TitleY;
    switch (self->Update) {
    case TextMenuUpdate_OneByOne:
        if (self->Cursor == 0) {
            Y = 0;

        } else if (self->Page->LowerPages[self->Cursor].Y < 1) {
            Y = self->Page->TitleY - self->Page->LowerPages[self->Cursor].Y + 1;

        } else if (self->Page->LowerPages[self->Cursor].Y +
                       self->Page->LowerPages[self->Cursor].Height >=
                   OLED->Height) {
            Y = self->Page->TitleY -
                (self->Page->LowerPages[self->Cursor].Y - OLED->Height +
                 self->Page->LowerPages[self->Cursor].Height) -
                1;
        }
        break;

    case TextMenuUpdate_PageByPage:
        if (self->PageNumber == 0) {
            Y = 0;

        } else {
            uint8_t Index = self->TextCountOfHomePage +
                            self->TextCountOfOtherPage * (self->PageNumber - 1);
            Y = self->Page->TitleY - self->Page->LowerPages[Index].Y + 1;
        }
        break;
    }

    PositionUpdate(self->Page->TitleY, Y);

    for (uint8_t i = 0; i < self->Page->NumOfLowerPages; i++) {
        if (i == 0) {
            PositionUpdate(self->Page->LowerPages[0].Y,
                           self->Page->TitleY + self->Page->TitleHeight + 1);
        } else {
            PositionUpdate(self->Page->LowerPages[i].Y,
                           self->Page->LowerPages[i - 1].Y +
                               self->Page->LowerPages[i - 1].Height + 2);
        }
    }
}

ErrorStatus TextMenu_CursorInc(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages) {
        self->Cursor = (self->Cursor + 1) % self->Page->NumOfLowerPages;
        self->Page->Cursor = self->Cursor;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_CursorDec(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages) {
        self->Cursor = (self->Cursor + self->Page->NumOfLowerPages - 1) %
                       self->Page->NumOfLowerPages;
        self->Page->Cursor = self->Cursor;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_EnterLowerPage(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages) {
        self->Page = &self->Page->LowerPages[self->Cursor];
        self->Cursor = 0;
        self->Page->Cursor = self->Cursor;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_ReturnUpperPage(TextMenu_t *self) {
    if (self->Page->UpperPage) {
        TextPage_SetY(self->Page, 0);
        self->Page = self->Page->UpperPage;
        self->Cursor = self->Page->Cursor;

        return SUCCESS;
    }

    return ERROR;
}

void ImageMenu_Init(ImageMenu_t *self, OLED_t *OLED, TextMenu_t *Menu) {
    for (uint8_t i = 0; i < self->NumOfPages; i++) {
        if (i == 0) {
            PositionUpdate(self->Page[0].ImageX,
                           OLED->Width / 2 - self->ImageWidth / 2);

        } else {
            PositionUpdate(self->Page[i].ImageX, self->Page[i - 1].ImageX +
                                                     self->ImageWidth +
                                                     self->Space);
        }

        self->Page[i].ImageY = OLED->Height / 2 - self->ImageHeight / 2;

        if (IsChinese(self->Page[i].Title)) {
            OLEDFont Font = OLED->Font;
            OLED_SetFont(OLED, OLEDFont_Chinese12X12);
            self->Page[i].TitleY =
                self->Page[i].ImageY + self->ImageHeight + OLED->FontHeight;
            self->Page[i].TitleWidth = strlen(self->Page[i].Title) /
                                       OLED_ChineseBytesCount * OLED->FontWidth;
            self->Page[i].TitleHeight = OLED->FontHeight;
            OLED_SetFont(OLED, Font);

        } else {
            self->Page[i].TitleY =
                self->Page[i].ImageY + self->ImageHeight + OLED->FontHeight;
            self->Page[i].TitleWidth =
                strlen(self->Page[i].Title) * OLED->FontWidth;
            self->Page[i].TitleHeight = OLED->FontHeight;
        }

        if (self->Page[i].TextPage) {
            TextPage_Init(self->Page[i].TextPage, OLED, Menu);
        }
    }
}

void ImageMenu_Update(ImageMenu_t *self, OLED_t *OLED) {
    int16_t X = self->Page[0].ImageX - (self->Page[self->Cursor].ImageX +
                                        self->ImageWidth / 2 - OLED->Width / 2);
    PositionUpdate(self->Page[0].ImageX, X);
    self->Page[0].TitleX = self->Page[0].ImageX + self->ImageWidth / 2 -
                           self->Page[0].TitleWidth / 2;

    for (uint8_t i = 1; i < self->NumOfPages; i++) {
        PositionUpdate(self->Page[i].ImageX, self->Page[i - 1].ImageX +
                                                 self->ImageWidth +
                                                 self->Space);

        self->Page[i].TitleX = self->Page[i].ImageX + self->ImageWidth / 2 -
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
    TextMenu->Cursor = 0;
    TextMenu->Page->Cursor = TextMenu->Cursor;

    return SUCCESS;
}

ErrorStatus ImageMenu_ReturnUpperPage(ImageMenu_t *self, TextMenu_t *TextMenu) {
    TextPage_SetY(TextMenu->Page, 0);

    return SUCCESS;
}

void SelectioneBar_BindTextPage(SelectioneBar_t *self, TextPage_t *Page) {
    self->TextX = &Page->X;
    self->TextY = &Page->Y;
    self->TextWidth = &Page->Width;
    self->TextHeight = &Page->Height;
}

void SelectioneBar_BindImagePage(SelectioneBar_t *self, ImagePage_t *Page) {
    self->TextX = &Page->TitleX;
    self->TextY = &Page->TitleY;
    self->TextWidth = &Page->TitleWidth;
    self->TextHeight = &Page->TitleHeight;
}

void SelectioneBar_Update(SelectioneBar_t *self) {
    PositionUpdate(self->X, *self->TextX - 1);
    PositionUpdate(self->Y, *self->TextY - 1);
    PositionUpdate(self->Width, *self->TextWidth + 2);
    PositionUpdate(self->Height, *self->TextHeight + 2);
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar) {
    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y,
                     SelectioneBar->Width, SelectioneBar->Height);
}