#include "main.h"

void TextPage_CursorCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        if (TextPage_CursorInc(TextMenu.Page)) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }

    } else if (Encoder <= -3) {
        if (TextPage_CursorDec(TextMenu.Page)) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }
    }
}

void TextPage_ThresholdCallback(int16_t Encoder) {
    MQSensor_UpdateThreshold(&MQSensor[TextMenu.Page->UpperPage->Cursor - 1],
                             Encoder > 0 ? -128 : +128);
}

void TextPage_SettingIncDecCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting++;

    } else if (Encoder <= -3) {
        TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting--;
    }
}

void ImagePage_CursorCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        if (ImageMenu_CursorInc(&ImageMenu)) {
            SelectioneBar_BindImagePage(&Bar,
                                        &ImageMenu.Page[ImageMenu.Cursor]);
        }

    } else if (Encoder <= -3) {
        if (ImageMenu_CursorDec(&ImageMenu)) {
            SelectioneBar_BindImagePage(&Bar,
                                        &ImageMenu.Page[ImageMenu.Cursor]);
        }
    }
}