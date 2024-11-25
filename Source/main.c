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

    MQSensor_Init(&MQ2Sensor);
    MQSensor_Init(&MQ3Sensor);
    MQSensor_Init(&MQ7Sensor);
    MQSensor_Init(&MQ135Sensor);

    Sampler_Init(&Sampler);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    TextMenu_Init(&TextMenu, &OLED);
    ImageMenu_Init(&ImageMenu, &OLED, &TextMenu);
    SelectioneBar_BindTextPage(&Bar, &TextMenu.Page->LowerPages[0]);

    HomeTextPage = TextMenu.Page;
    MQ2TextPage = &TextMenu.Page->LowerPages[0];
    MQ3TextPage = &TextMenu.Page->LowerPages[1];
    MQ7TextPage = &TextMenu.Page->LowerPages[2];
    MQ135TextPage = &TextMenu.Page->LowerPages[3];
    SettingTextPage = &TextMenu.Page->LowerPages[4];
    StatusLEDSetting = &SettingTextPage->LowerPages[0];
    ReverseSetting = &SettingTextPage->LowerPages[1];
    RestartSetting = &SettingTextPage->LowerPages[2];

    ImageMenu.Page[0].TextPage = HomeTextPage;
    ImageMenu.Page[1].TextPage = MQ2TextPage;
    ImageMenu.Page[2].TextPage = MQ3TextPage;
    ImageMenu.Page[3].TextPage = MQ7TextPage;
    ImageMenu.Page[4].TextPage = MQ135TextPage;
    ImageMenu.Page[5].TextPage = SettingTextPage;

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