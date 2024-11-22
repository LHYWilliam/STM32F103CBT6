#include "Menu.h"
#include "OLED.h"

void TextMenu_Init(TextMenu_t *self) { TextPage_Init(self->Page); }

void TextPage_Init(TextPage_t *self) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].UpperPage = self;
        TextPage_Init(&self->LowerPages[i]);
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

void SelectioneBar_Init(SelectioneBar_t *self, uint8_t X, uint8_t Y,
                        uint8_t Width, uint8_t Height, uint8_t Speed) {
    self->X = X;
    self->Y = Y;
    self->Width = Width;
    self->Height = Height;
    self->Speed = Speed;
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar,
                            uint8_t GoalY, uint8_t GoalWidth,
                            uint8_t GoalHeight) {
    int8_t dy = SelectioneBar->Y < GoalY   ? SelectioneBar->Speed
                : SelectioneBar->Y > GoalY ? -SelectioneBar->Speed
                                           : 0;
    int8_t dw = SelectioneBar->Width < GoalWidth   ? SelectioneBar->Speed
                : SelectioneBar->Width > GoalWidth ? -SelectioneBar->Speed
                                                   : 0;
    int8_t dh = SelectioneBar->Height < GoalHeight   ? SelectioneBar->Speed
                : SelectioneBar->Height > GoalHeight ? -SelectioneBar->Speed
                                                     : 0;

    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y += dy,
                     SelectioneBar->Width += dw, SelectioneBar->Height += dh);
}

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu) {
    uint8_t begin = Menu->Cursor >= Menu->NumOfTexts ? Menu->Cursor - 3 : 0;

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
