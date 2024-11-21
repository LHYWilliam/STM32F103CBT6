#include "main.h"

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

#define Samper_Length 2
uint16_t Data[Samper_Length];
Sampler_t Sampler = {
    .Data = Data,
    .Length = Samper_Length,

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