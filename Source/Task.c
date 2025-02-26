#include "main.h"

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    if (Menu == &TextMenu) {
        TextMenu.Page->ShowCallback(NULL);

    } else if (Menu == &ImageMenu) {
        ImageMenu.ShowCallback(NULL);
    }

    OLED_ShowSelectioneBar(&OLED, &Bar);

    if (ReverseSetting->Setting) {
        OLED_Reverse(&OLED);
    }

    OLED_SendBuffer(&OLED);
}

void vUpdateTimerCallback(TimerHandle_t pxTimer) {
    if (Menu == &TextMenu) {
        TextMenu.Page->UpdateCallback(NULL);

    } else if (Menu == &ImageMenu) {
        ImageMenu_Update(&ImageMenu, &OLED);
    }

    SelectioneBar_Update(&Bar);

    for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
        MQSensor_UpdateState(&MQSensor[i]);
    }

    if (LEDSetting->Setting) {
        LED_On(&LED);
    } else {
        LED_Off(&LED);
    }
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        int16_t Encode = Encoder_GetCount(&Encoder);

        if (Encode <= -3 || Encode >= 3) {
            if (Menu == &ImageMenu) {
                ImageMenu.RotationCallback(Encode);

            } else if (Menu == &TextMenu) {
                TextMenu.Page->LowerPages[TextMenu.Page->Cursor]
                    .RotationCallback(Encode);
            }
        }

        if (Key_Read(&KeyConfirm)) {
            if (Menu == &ImageMenu) {
                ImageMenu.ClickCallback(NULL);

            } else if (Menu == &TextMenu) {
                TextMenu.Page->LowerPages[TextMenu.Page->Cursor].ClickCallback(
                    NULL);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}