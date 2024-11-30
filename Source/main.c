#include "main.h"

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    LED_Init(&LED);
    Key_Init(&Key);
    W25Q64_Init(&W25Q64);

    Key_Init(&KeyConfirm);
    Encoder_Init(&Encoder);

    for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
        MQSensor_Init(&MQSensor[i]);
    }

    Sampler_Init(&Sampler);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    StatusLEDSetting = &SettingPage.LowerPages[1];
    ReverseSetting = &SettingPage.LowerPages[2];
    RestartSetting = &SettingPage.LowerPages[3];

    TextPage_Init(&MonitorPage, &OLED);
    TextPage_Init(&SettingPage, &OLED);

    TextMenu_Init(&TextMenu, &OLED);
    ImageMenu_Init(&ImageMenu, &OLED);
    SelectioneBar_BindImagePage(&Bar, &ImageMenu.Page[0]);
    // SelectioneBar_BindTextPage(&Bar, &TextMenu.Page->LowerPages[0]);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vUpdateTimer = xTimerCreate("vUpdateTimer", pdMS_TO_TICKS(10), pdTRUE,
                                (void *)0, vUpdateTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(10), pdTRUE,
                              (void *)1, vOLEDTimerCallback);

    xTimerStart(vUpdateTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    vTaskStartScheduler();
}