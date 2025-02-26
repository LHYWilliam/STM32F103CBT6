#include "main.h"

void TextPage_BackImageMenuCallback(void *pvParameters) {
    Menu = &ImageMenu;
    if (ImageMenu_ReturnUpperPage(&ImageMenu, &TextMenu)) {
        SelectioneBar_BindImagePage(&Bar, &ImageMenu.Page[ImageMenu.Cursor]);
    }
}

void ImagePage_EnterTextPageCallback(void *pvParameters) {
    Menu = &TextMenu;
    ImageMenu_EnterLowerPage(&ImageMenu, &TextMenu);
    SelectioneBar_BindTextPage(
        &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
}

void TextPage_EnterCallback(void *pvParameters) {
    if (TextPage_EnterLowerPage(&TextMenu.Page)) {
        SelectioneBar_BindTextPage(
            &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
    }
}

void TextPage_BackCallback(void *pvParameters) {
    if (TextPage_ReturnUpperPage(&TextMenu.Page)) {
        SelectioneBar_BindTextPage(
            &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
    }
}

void Setting_RestartCallback(void *pvParameters) { __NVIC_SystemReset(); }

void Setting_ReverseCallback(void *pvParameters) {
    TextPage_ReverseSetting(TextMenu.Page);
}

void Setting_CursorSwitchIncDecCallback(void *pvParameters) {
    if (TextMenu.Page->LowerPages[TextMenu.Page->Cursor].RotationCallback ==
        TextPage_CursorCallback) {
        TextMenu.Page->LowerPages[TextMenu.Page->Cursor].RotationCallback =
            Setting_IncDecCallback;

    } else if (TextMenu.Page->LowerPages[TextMenu.Page->Cursor]
                   .RotationCallback == Setting_IncDecCallback) {
        TextMenu.Page->LowerPages[TextMenu.Page->Cursor].RotationCallback =
            TextPage_CursorCallback;
    }
}

void Setting_SaveCallback(void *pvParameters) {
    uint8_t Setting[32];
    for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
        Setting[i - 1] = SettingPage.LowerPages[i].Setting;
    }

    W25Q64_SectorErase(&W25Q64, 0);
    W25Q64_PageProgram(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

    TextPage_EnterCallback(NULL);
}

void SettingLoad(void *pvParameters) {
    uint8_t Setting[32];
    W25Q64_ReadData(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

    for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
        SettingPage.LowerPages[i].Setting = Setting[i - 1];
    }
}

void Setting_LoadCallback(void *pvParameters) {
    SettingLoad(pvParameters);

    TextPage_EnterCallback(NULL);
}