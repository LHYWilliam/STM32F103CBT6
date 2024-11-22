#ifndef MENU_H
#define MENU_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

typedef struct TextPage {
    char Title[32];

    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    uint8_t Space;

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

    uint8_t Speed;
} SelectioneBar_t;

typedef struct {
    TextPage_t *Page;
    uint8_t PageNumber;

    uint8_t Speed;

    uint8_t Cursor;
    SelectioneBar_t Bar;
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

void TextPage_Init(TextPage_t *self);

void TextMenu_Init(TextMenu_t *self);
void TextMenu_Update(TextMenu_t *self, int16_t Y);
void TextMenu_CursorInc(TextMenu_t *self);
void TextMenu_CursorDec(TextMenu_t *self);
void TextMenu_EnterLowerPage(TextMenu_t *self);
void TextMenu_ReturnUpperPage(TextMenu_t *self);

void SelectioneBar_Init(SelectioneBar_t *self, int16_t X, int16_t Y,
                        uint8_t Width, uint8_t Height, uint8_t Speed);
void SelectioneBar_Update(SelectioneBar_t *self, int16_t Y, uint8_t Width,
                          uint8_t Height);

void ImageMenu_CursorInc(ImageMenu_t *self);
void ImageMenu_CursorDec(ImageMenu_t *self);

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu);
void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu);
void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar);

#endif