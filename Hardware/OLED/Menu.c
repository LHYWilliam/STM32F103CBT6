#include "Menu.h"

void TextMenu_Init(TextMenu_t *self) { TextPage_Init(self->Page); }

void TextPage_Init(TextPage_t *self) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].UpperPage = self;
        TextPage_Init(&self->LowerPages[i]);
    }
}

void TextMenu_CursorInc(TextMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->Page->NumOfLowerPages;
}

void TextMenu_CursorDec(TextMenu_t *self) {
    if (self->Cursor == 0) {
        self->Cursor = self->Page->NumOfLowerPages - 1;
    } else {
        self->Cursor--;
    }
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
        self->Cursor = 0;
    }
}

void ImageMenu_CursorInc(ImageMenu_t *self) {
    self->Cursor = (self->Cursor + 1) % self->NumOfPages;
}

void ImageMenu_CursorDec(ImageMenu_t *self) {
    if (self->Cursor == 0) {
        self->Cursor = self->NumOfPages - 1;
    } else {
        self->Cursor--;
    }
}

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu) {
    uint8_t begin = Menu->Cursor >= TEXT_COUNT_OF_PAGE ? Menu->Cursor - 3 : 0;

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
