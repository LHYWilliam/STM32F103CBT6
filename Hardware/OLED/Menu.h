#ifndef MENU_H
#define MENU_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

typedef struct TextPage {
    char Title[32];

    uint8_t Cursor;

    uint8_t NumOfLowerPages;
    struct TextPage *LowerPages;
    struct TextPage *UpperPage;
} TextPage_t;

typedef struct {
    uint8_t X;
    uint8_t Y;
    uint8_t Width;
    uint8_t Height;

    uint8_t Speed;
} SelectioneBar_t;

typedef struct {
    TextPage_t *Page;
    uint8_t NumOfTexts;

    uint8_t Cursor;
    SelectioneBar_t Bar;
} TextMenu_t;

typedef struct {
    const uint8_t *Image;

    TextPage_t *LowerPages;
} ImagePage_t;

typedef struct {
    uint8_t NumOfPages;
    ImagePage_t *Page;

    uint8_t Cursor;
} ImageMenu_t;

void TextMenu_Init(TextMenu_t *self);
void TextPage_Init(TextPage_t *self);
void TextMenu_CursorInc(TextMenu_t *self);
void TextMenu_CursorDec(TextMenu_t *self);
void TextMenu_EnterLowerPage(TextMenu_t *self);
void TextMenu_ReturnUpperPage(TextMenu_t *self);

void SelectioneBar_Init(SelectioneBar_t *self, uint8_t X, uint8_t Y,
                        uint8_t Width, uint8_t Height, uint8_t Speed);

void ImageMenu_CursorInc(ImageMenu_t *self);
void ImageMenu_CursorDec(ImageMenu_t *self);

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar,
                            uint8_t GoalY);

void OLED_ShowTextMenu(OLED_t *OLED, TextMenu_t *Menu);
void OLED_ShowImageMenu(OLED_t *OLED, ImageMenu_t *Menu);

#endif