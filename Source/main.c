#include "main.h"

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    RTC_Init();

    LED_Init(&LED);
    Key_Init(&Key);
    Serial_Init(&Serial);

    // Sampler_Init(&Sampler);
    // Encoder_Init(&Encoder);

    // OLED_Init(&OLED);
    // OLED_SetFont(&OLED, OLEDFont_6X8);

    // LCD_Init(&LCD);
    // LCD_SetFont(&LCD, (uint32_t)LCD_Font8x16);

    // W25Q64_Init(&W25Q64);
    // for (uint8_t i = 0; i < sizeof(MQSensor) / sizeof(MQSensor[0]); i++) {
    //     MQSensor_Init(&MQSensor[i]);
    // }

    ICM42688_Init(&ICM42688);

    // GrayScaleSensor_Init(&GrayScaleSensor);

    xTaskCreate(vMainTaskCode, "vMainTask", 128, NULL, 1, &xMainTaskHandle);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);

    xTimerStart(vLEDTimer, 0);

    vTaskStartScheduler();
}