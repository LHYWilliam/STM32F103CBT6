#ifndef MENU_H
#define MENU_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

#define TextMenu_PageNumber(Menu)                                              \
    ((Menu->Cursor - 1) < Menu->TextCountOfHomePage                            \
         ? 0                                                                   \
         : ((Menu->Cursor - 1) - Menu->TextCountOfHomePage) /                  \
                   Menu->TextCountOfOtherPage +                                \
               1)

#define PositionUpdate(now, target)                                            \
    ((now) += ((now) < (target) ? 1 : (now) > (target) ? -1 : 0))

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

    uint8_t Cursor;
    uint8_t NumOfLowerPages;
    struct TextPage *LowerPages;
    struct TextPage *UpperPage;

    int16_t Setting;

    void (*ShowCallback)(void *);
    void (*ClickCallback)(void *);
    void (*RotationCallback)(int16_t);
} TextPage_t;

typedef enum {
    TextMenuUpdate_OneByOne,
    TextMenuUpdate_PageByPage,
} TextMenuUpdate;

typedef struct {
    uint8_t Cursor;
    TextPage_t *Page;

    uint8_t PageNumber;
    uint8_t TextCountOfHomePage;
    uint8_t TextCountOfOtherPage;

    TextMenuUpdate Update;
} TextMenu_t;

typedef struct {
    char Title[32];
    const uint8_t *Image;

    int16_t ImageX;
    int16_t ImageY;
    uint8_t ImageWidth;
    uint8_t ImageHeight;

    int16_t TitleX;
    int16_t TitleY;
    uint8_t TitleWidth;
    uint8_t TitleHeight;

    TextPage_t *TextPage;
} ImagePage_t;

typedef struct {
    uint8_t Cursor;
    ImagePage_t *Page;
    uint8_t NumOfPages;

    uint8_t ImageWidth;
    uint8_t ImageHeight;

    uint8_t Space;

    void (*ShowCallback)(void *);
    void (*ClickCallback)(void *);
    void (*RotationCallback)(int16_t);
} ImageMenu_t;

typedef struct {
    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    int16_t *TargetX;
    int16_t *TargetY;
    uint8_t *TargetWidth;
    uint8_t *TargetHeight;
} SelectioneBar_t;

void TextPage_Init(TextPage_t *self, OLED_t *OLED);
void TextPage_ResetSetY(TextPage_t *self);
void TextPage_ReverseSetting(TextPage_t *self);

void TextMenu_Init(TextMenu_t *self, OLED_t *OLED);
void TextMenu_Update(TextMenu_t *self, OLED_t *OLED);
ErrorStatus TextMenu_CursorInc(TextMenu_t *self);
ErrorStatus TextMenu_CursorDec(TextMenu_t *self);
ErrorStatus TextMenu_EnterLowerPage(TextMenu_t *self);
ErrorStatus TextMenu_ReturnUpperPage(TextMenu_t *self);

void ImagePage_Init(ImagePage_t *self, OLED_t *OLED);

void ImageMenu_Init(ImageMenu_t *self, OLED_t *OLED);
void ImageMenu_Update(ImageMenu_t *self, OLED_t *OLED);
ErrorStatus ImageMenu_CursorInc(ImageMenu_t *self);
ErrorStatus ImageMenu_CursorDec(ImageMenu_t *self);
ErrorStatus ImageMenu_EnterLowerPage(ImageMenu_t *ImageMenu,
                                     TextMenu_t *TextMenu);
ErrorStatus ImageMenu_ReturnUpperPage(ImageMenu_t *self, TextMenu_t *TextMenu);

void SelectioneBar_BindTextPage(SelectioneBar_t *self, TextPage_t *Page);
void SelectioneBar_BindImagePage(SelectioneBar_t *self, ImagePage_t *Page);
void SelectioneBar_Update(SelectioneBar_t *self);
void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar);

#endif