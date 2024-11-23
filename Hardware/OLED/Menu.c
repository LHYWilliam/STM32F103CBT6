#include <string.h>

#include "Menu.h"
#include "OLED.h"

#define Update(now, target, speed)                                             \
    ((now) += ((now) < (target) ? (speed) : (now) > (target) ? -(speed) : 0))

void TextPage_Init(TextPage_t *self, OLED_t *OLED, TextMenu_t *Menu) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].X += 1;
        self->LowerPages[i].Width =
            strlen(self->LowerPages[i].Title) * OLED->FontWidth;
        self->LowerPages[i].Height = OLED->FontHeight;

        if (i == 0) {
            Update(self->LowerPages[0].Y, self->TitleY + self->TitleHeight + 1,
                   Menu->Speed);
        } else {
            Update(self->LowerPages[i].Y,
                   self->LowerPages[i - 1].Y + self->LowerPages[i - 1].Height +
                       2,
                   Menu->Speed);
        }

        self->LowerPages[i].UpperPage = self;

        TextPage_Init(&self->LowerPages[i], OLED, Menu);
    }
}

void TextPage_Update(TextPage_t *self, TextMenu_t *Menu) {
    Menu->PageNumber = TextMenu_PageNumber(Menu);

    int16_t Y =
        Menu->PageNumber == 0
            ? 0
            : (self->TitleY - (self->LowerPages[Menu->TextCountOfHomePage +
                                                Menu->TextCountOfOtherPage *
                                                    (Menu->PageNumber - 1)]
                                   .Y -
                               1));
    Update(self->TitleY, Y, Menu->Speed);

    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        if (i == 0) {
            Update(self->LowerPages[0].Y, self->TitleY + self->TitleHeight + 1,
                   Menu->Speed);
        } else {
            Update(self->LowerPages[i].Y,
                   self->LowerPages[i - 1].Y + self->LowerPages[i - 1].Height +
                       2,
                   Menu->Speed);
        }
    }
}

void TextPage_ReverseSetting(TextPage_t *self) {
    self->LowerPages[self->Cursor].Setting =
        !self->LowerPages[self->Cursor].Setting;
}

void TextMenu_Init(TextMenu_t *self, OLED_t *OLED) {
    TextPage_Init(self->Page, OLED, self);
    SelectioneBar_Bind(&self->Bar, &self->Page->LowerPages[0]);
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

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextMenu_ReturnUpperPage(TextMenu_t *self) {
    if (self->Page->UpperPage) {
        self->Page = self->Page->UpperPage;
        self->Cursor = self->Page->Cursor;

        return SUCCESS;
    }

    return ERROR;
}

void ImageMenu_CursorInc(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->NumOfPages;
}

void ImageMenu_CursorDec(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + self->NumOfPages - 1) % self->NumOfPages;
}

void SelectioneBar_Bind(SelectioneBar_t *self, TextPage_t *Page) {
    self->TextX = &Page->X;
    self->TextY = &Page->Y;
    self->TextWidth = &Page->Width;
    self->TextHeight = &Page->Height;
}

void SelectioneBar_Update(SelectioneBar_t *self) {
    Update(self->X, *self->TextX - 1, self->Speed);
    Update(self->Y, *self->TextY - 1, self->Speed);
    Update(self->Width, *self->TextWidth + 2, self->Speed);
    Update(self->Height, *self->TextHeight + 2, self->Speed);
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar) {
    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y,
                     SelectioneBar->Width, SelectioneBar->Height);
}

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu) {
    uint8_t begin =
        Menu->Cursor >= Menu->TextCountOfHomePage ? Menu->Cursor - 3 : 0;
    for (uint8_t i = 0; i < Menu->Page->NumOfLowerPages; i++) {
        if (begin + i == Menu->Cursor) {
            OLED_Printf(OLED, 0, i * 16, "%s <-",
                        Menu->Page->LowerPages[begin + i].Title);
        } else {
            OLED_Printf(OLED, 0, i * 16, "%s",
                        Menu->Page->LowerPages[begin + i].Title);
        }
    }
}

void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu) {
    OLED_ShowImage(
        OLED, 0, 16, 32, 32,
        Menu->Page[Menu->Cursor != 0 ? Menu->Cursor - 1 : Menu->NumOfPages - 1]
            .Image);

    OLED_ShowImage(OLED, 48, 16, 32, 32, Menu->Page[Menu->Cursor].Image);

    OLED_ShowImage(
        OLED, 96, 16, 32, 32,
        Menu->Page[Menu->Cursor != Menu->NumOfPages - 1 ? Menu->Cursor + 1 : 0]
            .Image);
}
