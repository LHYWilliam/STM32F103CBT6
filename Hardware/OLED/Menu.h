#ifndef MENU_H
#define MENU_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

#define TextMenu_PageNumber(Menu)                                              \
    (Menu->Cursor < Menu->TextCountOfHomePage                                  \
         ? 0                                                                   \
         : (Menu->Cursor - Menu->TextCountOfHomePage) /                        \
                   Menu->TextCountOfOtherPage +                                \
               1)

typedef struct TextPage {
    char Title[32];

    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    int16_t TitleX;
    int16_t TitleY;
    uint8_t TitleWidth;
    uint8_t TitleHeight;

    uint8_t Setting;

    uint8_t Cursor;
    uint8_t NumOfLowerPages;
    struct TextPage *LowerPages;
    struct TextPage *UpperPage;
} TextPage_t;

typedef struct {
    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    int16_t *TextX;
    int16_t *TextY;
    uint8_t *TextWidth;
    uint8_t *TextHeight;

    uint8_t Speed;
} SelectioneBar_t;

typedef struct {
    TextPage_t *Page;
    uint8_t PageNumber;

    uint8_t Speed;

    uint8_t Cursor;
    uint8_t TextCountOfHomePage;
    uint8_t TextCountOfOtherPage;
} TextMenu_t;

typedef struct {
    const uint8_t *Image;

    TextPage_t *LowerPages;
} ImagePage_t;

typedef struct {
    ImagePage_t *Page;

    uint8_t Cursor;
    uint8_t NumOfPages;
} ImageMenu_t;

void TextPage_Init(TextPage_t *self, OLED_t *OLED, TextMenu_t *Menu);
void TextPage_Update(TextPage_t *self, TextMenu_t *Menu);
void TextPage_ReverseSetting(TextPage_t *self);

void SelectioneBar_Bind(SelectioneBar_t *self, TextPage_t *Page);
void SelectioneBar_Update(SelectioneBar_t *self);

void TextMenu_Init(TextMenu_t *self, OLED_t *OLED);
ErrorStatus TextMenu_CursorInc(TextMenu_t *self);
ErrorStatus TextMenu_CursorDec(TextMenu_t *self);
ErrorStatus TextMenu_EnterLowerPage(TextMenu_t *self);
ErrorStatus TextMenu_ReturnUpperPage(TextMenu_t *self);

void ImageMenu_CursorInc(ImageMenu_t *self);
void ImageMenu_CursorDec(ImageMenu_t *self);

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu);
void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu);
void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar);

#endif