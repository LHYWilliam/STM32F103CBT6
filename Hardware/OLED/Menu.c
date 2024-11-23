#include "Menu.h"
#include "OLED.h"

#define Update(now, goal, speed)                                               \
    ((now) += ((now) < (goal) ? (speed) : (now) > (goal) ? -(speed) : 0))

void TextMenu_Init(TextMenu_t *self) { TextPage_Init(self->Page); }

void TextPage_Init(TextPage_t *self) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        if (i == 0) {
            Update(self->LowerPages[0].Y, self->TitleY + self->TitleHeight + 1,
                   1);
        } else {
            Update(self->LowerPages[i].Y,
                   self->LowerPages[i - 1].Y + self->LowerPages[i - 1].Height +
                       2,
                   1);
        }

        self->LowerPages[i].UpperPage = self;
        TextPage_Init(&self->LowerPages[i]);
    }
}

void TextMenu_Update(TextMenu_t *self, int16_t Y) {
    Update(self->Page->TitleY, Y, self->Speed);

    for (uint8_t i = 0; i < self->Page->NumOfLowerPages; i++) {
        if (i == 0) {
            Update(self->Page->LowerPages[0].Y,
                   self->Page->TitleY + self->Page->TitleHeight + 1,
                   self->Speed);
        } else {
            Update(self->Page->LowerPages[i].Y,
                   self->Page->LowerPages[i - 1].Y +
                       self->Page->LowerPages[i - 1].Height + 2,
                   self->Speed);
        }
    }
}

void TextMenu_CursorInc(TextMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->Page->NumOfLowerPages;
    self->Page->Cursor = self->Cursor;
}

void TextMenu_CursorDec(TextMenu_t *self) {
    self->Cursor = (self->Cursor + self->Page->NumOfLowerPages - 1) %
                   self->Page->NumOfLowerPages;
    self->Page->Cursor = self->Cursor;
}

void TextMenu_EnterLowerPage(TextMenu_t *self) {
    if (self->Page->NumOfLowerPages) {
        self->Page = &self->Page->LowerPages[self->Cursor];
        self->Cursor = 0;
    }
}

void TextMenu_ReturnUpperPage(TextMenu_t *self) {
    if (self->Page->UpperPage) {
        self->Page = self->Page->UpperPage;
        self->Cursor = self->Page->Cursor;
    }
}

void ImageMenu_CursorInc(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->NumOfPages;
}

void ImageMenu_CursorDec(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + self->NumOfPages - 1) % self->NumOfPages;
}

void SelectioneBar_Init(SelectioneBar_t *self, int16_t X, int16_t Y,
                        uint8_t Width, uint8_t Height, uint8_t Speed) {
    self->X = X;
    self->Y = Y;
    self->Width = Width;
    self->Height = Height;
    self->Speed = Speed;
}

void SelectioneBar_Update(SelectioneBar_t *self, int16_t X, int16_t Y,
                          uint8_t Width, uint8_t Height) {
    Update(self->X, X, self->Speed);
    Update(self->Y, Y, self->Speed);
    Update(self->Width, Width, self->Speed);
    Update(self->Height, Height, self->Speed);
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
