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
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

MQSensor_t MQ135 = {
    .LED = B1,
    .Mode = LEDMode_Low,
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

#define Samper_DataLength 2
uint16_t Data[Samper_DataLength];
Sampler_t Sampler = {
    .Data = Data,
    .Length = Samper_DataLength,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

#define TextHeight   8
#define TitleHeight_ 20

TextMenu_t Menu = {
    .Speed = 1,
    .TextCountOfHomePage = 4,
    .TextCountOfOtherPage = 6,
    .Bar =
        {
            .Width = 1,
            .Height = 1,
            .Speed = 1,
        },
    .Page =
        &(TextPage_t){
            .Title = "异味检测与开窗系统",
            .TitleHeight = TitleHeight_,
            .NumOfLowerPages = 11,
            .LowerPages =
                (TextPage_t[]){
                    (TextPage_t){
                        .Title = "MQ-3",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "MQ-135",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "MQ-...",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "Setting",
                        .TitleHeight = TitleHeight_,
                        .Height = TextHeight,
                        .NumOfLowerPages = 11,
                        .LowerPages =
                            (TextPage_t[]){
                                (TextPage_t){
                                    .Title = "Reverse",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "Reset",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                                (TextPage_t){
                                    .Title = "...",
                                    .Height = TextHeight,
                                },
                            },
                    },
                    (TextPage_t){
                        .Title = "5",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "6",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "7",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "8",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "9",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "10",
                        .Height = TextHeight,
                    },
                    (TextPage_t){
                        .Title = "11",
                        .Height = TextHeight,
                    },
                },
        },
};

TextPage_t *HomePage;
TextPage_t *MQ3Page;
TextPage_t *MQ135Page;
TextPage_t *SettingPage;

TextPage_t *ReverseSetting;
TextPage_t *ResetSetting;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vLEDTimer;
TimerHandle_t vOLEDTimer;
TimerHandle_t vMQSensorTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);
void vMQSensorTimerCallback(TimerHandle_t pxTimer);