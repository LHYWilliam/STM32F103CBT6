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
    HomePage = Menu.Page;
    MQ3Page = &Menu.Page->LowerPages[0];
    MQ135Page = &Menu.Page->LowerPages[1];
    SettingPage = &Menu.Page->LowerPages[3];
    ReverseSetting = &SettingPage->LowerPages[0];
    ResetSetting = &SettingPage->LowerPages[1];

    Sampler_Init(&Sampler);

    MQSensor_Init(&MQ3);
    MQSensor_Init(&MQ135);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(10), pdTRUE,
                              (void *)1, vOLEDTimerCallback);
    vMQSensorTimer = xTimerCreate("vMQSensorTimer", pdMS_TO_TICKS(100), pdTRUE,
                                  (void *)2, vMQSensorTimerCallback);

    xTimerStart(vLEDTimer, 0);
    xTimerStart(vOLEDTimer, 0);
    xTimerStart(vMQSensorTimer, 0);

    vTaskStartScheduler();
}