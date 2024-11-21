#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "MQSensor.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = LEDMode_High,
};

Key_t Key = {
    .GPIOxPiny = A0,
    .Mode = KeyMode_High,
};

Key_t KeyUp = {
    .GPIOxPiny = B15,
    .Mode = KeyMode_Low,
};

Key_t KeyDown = {
    .GPIOxPiny = A9,
    .Mode = KeyMode_Low,
};

Key_t KeyConfirm = {
    .GPIOxPiny = A11,
    .Mode = KeyMode_Low,
};

Key_t KeyCancel = {
    .GPIOxPiny = B13,
    .Mode = KeyMode_Low,
};

OLED_t OLED = {
    .SCL = B8,
    .SDA = B9,
    .I2C = ENABLE,
    .Width = 128,
    .Height = 64,
};

MQSensor_t MQ3 = {
    .LED = B11,
    .Mode = LEDMode_Low,
    .Threshold = 2.048 / 3.3 * 4095,
    .Relaxation = 0.128 / 3.3 * 4095,
};

MQSensor_t MQ135 = {
    .LED = B1,
    .Mode = LEDMode_Low,
    .Threshold = 2.048 / 3.3 * 4095,
    .Relaxation = 0.128 / 3.3 * 4095,
};

#define LENGTH 2
uint16_t Data[LENGTH];
Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

TextMenu_t Menu = {
    .Page =
        &(TextPage_t){
            .Title = "Home Page",
            .NumOfLowerPages = 2,
            .LowerPages =
                (TextPage_t[]){
                    (TextPage_t){
                        .Title = "MQ-3",
                    },
                    (TextPage_t){
                        .Title = "MQ-135",
                    },
                },
        },
};
TextPage_t *HomePage;
TextPage_t *MQ3Page;
TextPage_t *MQ135Page;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vLEDTimer;
TimerHandle_t vOLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);

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

    TextMenu_Init(&Menu);
    HomePage = Menu.Page;
    MQ3Page = &Menu.Page->LowerPages[0];
    MQ135Page = &Menu.Page->LowerPages[1];

    Sampler_Init(&Sampler);

    MQSensor_Init(&MQ3);
    MQSensor_Init(&MQ135);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(100), pdTRUE,
                              (void *)2, vOLEDTimerCallback);

    xTimerStart(vLEDTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    vTaskStartScheduler();
}