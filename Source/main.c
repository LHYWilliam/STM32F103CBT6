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

    for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
        MQSensor_Init(&MQSensor[i]);
    }

    Sampler_Init(&Sampler);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    ImageMenu.Page[0].TextPage = &MonitorPage;
    ImageMenu.Page[1].TextPage = &MQxChartPage;
    ImageMenu.Page[2].TextPage = &SettingPage;

    StatusLEDSetting = &SettingPage.LowerPages[0];
    ReverseSetting = &SettingPage.LowerPages[1];
    RestartSetting = &SettingPage.LowerPages[2];

    TextPage_Init(&MonitorPage, &OLED, &TextMenu);
    TextPage_Init(&MQxChartPage, &OLED, &TextMenu);
    TextPage_Init(&SettingPage, &OLED, &TextMenu);

    TextMenu_Init(&TextMenu, &OLED);
    ImageMenu_Init(&ImageMenu, &OLED);
    SelectioneBar_BindImagePage(&Bar, &ImageMenu.Page[0]);
    // SelectioneBar_BindTextPage(&Bar, &TextMenu.Page->LowerPages[0]);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vStateTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE,
                               (void *)0, vStateTimerCallback);
    vUpdateTimer = xTimerCreate("vUpdateTimer", pdMS_TO_TICKS(5), pdTRUE,
                                (void *)1, vUpdateTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(10), pdTRUE,
                              (void *)2, vOLEDTimerCallback);

    xTimerStart(vStateTimer, 0);
    xTimerStart(vUpdateTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    vTaskStartScheduler();
}