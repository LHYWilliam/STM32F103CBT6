#include "Menu.h"
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

MQSensor_t MQ2 = {
    .LED = A5,
    .Mode = LEDMode_Low,
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

MQSensor_t MQ3 = {
    .LED = A7,
    .Mode = LEDMode_Low,
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

MQSensor_t MQ7 = {
    .LED = B1,
    .Mode = LEDMode_Low,
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

MQSensor_t MQ135 = {
    .LED = B11,
    .Mode = LEDMode_Low,
    .Threshold = VoltageToADC(2.048),
    .Relaxation = VoltageToADC(0.128),
};

#define Samper_DataLength 4
uint16_t Data[Samper_DataLength];
Sampler_t Sampler = {
    .Data = Data,
    .Length = Samper_DataLength,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2 | 3 | 4",
    .GPIOxPiny = "A1 | A2 | A3 | A4",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

TextMenu_t Menu = {
    .Update = TextMenuUpdate_OneByOne,
    .TextCountOfHomePage = 4,
    .TextCountOfOtherPage = 6,
    .Page =
        &(TextPage_t){
            .Title = "异味检测与开窗系统",
            .NumOfLowerPages = 5,
            .LowerPages =
                (TextPage_t[]){
                    (TextPage_t){
                        .Title = "MQ-2",
                        .TitleX = 0,
                        .TitleY = 64 / 4,
                        .TitleWidth = 128,
                        .TitleHeight = 64 / 2,
                    },
                    (TextPage_t){
                        .Title = "MQ-3",
                        .TitleX = 0,
                        .TitleY = 64 / 4,
                        .TitleWidth = 128,
                        .TitleHeight = 64 / 2,
                    },
                    (TextPage_t){
                        .Title = "MQ-7",
                        .TitleX = 0,
                        .TitleY = 64 / 4,
                        .TitleWidth = 128,
                        .TitleHeight = 64 / 2,
                    },
                    (TextPage_t){
                        .Title = "MQ-135",
                        .TitleX = 0,
                        .TitleY = 64 / 4,
                        .TitleWidth = 128,
                        .TitleHeight = 64 / 2,
                    },
                    (TextPage_t){
                        .Title = "Setting",
                        .NumOfLowerPages = 3,
                        .LowerPages =
                            (TextPage_t[]){
                                (TextPage_t){
                                    .Title = "Status LED",
                                    .Setting = SET,
                                },
                                (TextPage_t){
                                    .Title = "Reverse",
                                },
                                (TextPage_t){
                                    .Title = "Restart",
                                },
                            },
                    },
                },
        },
};

ImageMenu_t ImageMenu = {
    .NumOfPages = 2,
    .ImageWidth = 32,
    .ImageHeight = 32,
    .Space = 16,
    .Page =
        (ImagePage_t[]){
            (ImagePage_t){
                .Title = "Home Page",
                .Image = MenuImage[0],
            },
            (ImagePage_t){
                .Title = "Setting Page",
                .Image = MenuImage[1],
            },
        },
};

SelectioneBar_t Bar;

TextPage_t *HomePage;
TextPage_t *MQ2Page;
TextPage_t *MQ3Page;
TextPage_t *MQ7Page;
TextPage_t *MQ135Page;
TextPage_t *SettingPage;

TextPage_t *StatusLEDSetting;
TextPage_t *ReverseSetting;
TextPage_t *RestartSetting;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vStateTimer;
TimerHandle_t vOLEDTimer;
void vStateTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);