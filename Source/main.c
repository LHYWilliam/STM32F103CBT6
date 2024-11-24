#include "main.h"

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    LED_Init(&LED);
    Key_Init(&Key);

    Key_Init(&KeyUp);
    Key_Init(&KeyDown);
    Key_Init(&KeyConfirm);
    Key_Init(&KeyCancel);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    TextMenu_Init(&Menu, &OLED);
    ImageMenu_Init(&ImageMenu, &OLED, &Menu);
    SelectioneBar_BindTextPage(&Bar, &Menu.Page->LowerPages[0]);
    HomePage = Menu.Page;
    MQ2Page = &Menu.Page->LowerPages[0];
    MQ3Page = &Menu.Page->LowerPages[1];
    MQ7Page = &Menu.Page->LowerPages[2];
    MQ135Page = &Menu.Page->LowerPages[3];
    SettingPage = &Menu.Page->LowerPages[4];
    StatusLEDSetting = &SettingPage->LowerPages[0];
    ReverseSetting = &SettingPage->LowerPages[1];
    RestartSetting = &SettingPage->LowerPages[2];

    Sampler_Init(&Sampler);

    MQSensor_Init(&MQ2);
    MQSensor_Init(&MQ3);
    MQSensor_Init(&MQ7);
    MQSensor_Init(&MQ135);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vStateTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE,
                               (void *)0, vStateTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(10), pdTRUE,
                              (void *)1, vOLEDTimerCallback);

    xTimerStart(vStateTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    // for (;;) {
    //     ImageMenu_Update(&ImageMenu, &OLED);

    //     OLED_ClearBuffer(&OLED);

    //     for (uint8_t i = 0; i < ImageMenu.NumOfPages; i++) {
    //         if (ImageMenu.Page[i].X + ImageMenu.ImageWidth < 0) {
    //             continue;
    //         }
    //         if (ImageMenu.Page[i].X >= OLED.Width) {
    //             break;
    //         }

    //         OLED_ShowImage(&OLED, ImageMenu.Page[i].X, ImageMenu.Page[i].Y,
    //                        ImageMenu.ImageWidth, ImageMenu.ImageHeight,
    //                        ImageMenu.Page[i].Image);

    //         OLED_Printf(&OLED, ImageMenu.Page[i].TitleX,
    //                     ImageMenu.Page[i].TitleY, "%s",
    //                     ImageMenu.Page[i].Title);
    //     }

    //     OLED_SendBuffer(&OLED);

    //     if (Key_Read(&KeyDown)) {
    //         ImageMenu_CursorInc(&ImageMenu);
    //     }

    //     if (Key_Read(&KeyUp)) {
    //         ImageMenu_CursorDec(&ImageMenu);
    //     }
    // }

    vTaskStartScheduler();
}