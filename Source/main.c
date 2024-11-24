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

    vTaskStartScheduler();
}