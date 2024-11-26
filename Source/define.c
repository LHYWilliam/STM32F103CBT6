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

MQSensor_t MQSensor[4] = {
    (MQSensor_t){
        .LED = A5,
        .Mode = LEDMode_Low,
        .Threshold = VoltageToADC(2.048),
        .Relaxation = VoltageToADC(0.128),
    },

    (MQSensor_t){
        .LED = A7,
        .Mode = LEDMode_Low,
        .Threshold = VoltageToADC(2.048),
        .Relaxation = VoltageToADC(0.128),
    },

    (MQSensor_t){
        .LED = B1,
        .Mode = LEDMode_Low,
        .Threshold = VoltageToADC(2.048),
        .Relaxation = VoltageToADC(0.128),
    },

    (MQSensor_t){
        .LED = B11,
        .Mode = LEDMode_Low,
        .Threshold = VoltageToADC(2.048),
        .Relaxation = VoltageToADC(0.128),
    },
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

TextPage_t MonitorPage = {
    .Title = "异味检测与开窗系统",
    .NumOfLowerPages = 4,
    .LowerPages =
        (TextPage_t[]){
            (TextPage_t){
                .Title = "MQ-2",
            },
            (TextPage_t){
                .Title = "MQ-3",
            },
            (TextPage_t){
                .Title = "MQ-7",
            },
            (TextPage_t){
                .Title = "MQ-135",
            },
        },
};

TextPage_t MQxChartPage = {
    .Title = "MQx Chart",
    .NumOfLowerPages = 4,
    .LowerPages =
        (TextPage_t[]){
            (TextPage_t){
                .Title = "MQ-2",
                .TitleX = 1,
                .TitleY = 64 / 4,
                .TitleWidth = 128 - 1,
                .TitleHeight = 64 / 2,
            },
            (TextPage_t){
                .Title = "MQ-3",
                .TitleX = 1,
                .TitleY = 64 / 4,
                .TitleWidth = 128 - 1,
                .TitleHeight = 64 / 2,
            },
            (TextPage_t){
                .Title = "MQ-7",
                .TitleX = 1,
                .TitleY = 64 / 4,
                .TitleWidth = 128 - 1,
                .TitleHeight = 64 / 2,
            },
            (TextPage_t){
                .Title = "MQ-135",
                .TitleX = 1,
                .TitleY = 64 / 4,
                .TitleWidth = 128 - 1,
                .TitleHeight = 64 / 2,
            },
        },
};

TextPage_t SettingPage = {
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
};

ImagePage_t HomePage[3] = {
    (ImagePage_t){
        .Title = "Montior",
        .Image = MenuImage[0],
        .TextPage = &MonitorPage,
    },
    (ImagePage_t){
        .Title = "Chart",
        .Image = MenuImage[1],
        .TextPage = &MQxChartPage,
    },
    (ImagePage_t){
        .Title = "Setting",
        .Image = MenuImage[2],
        .TextPage = &SettingPage,
    },
};

TextMenu_t TextMenu = {
    .Update = TextMenuUpdate_OneByOne,
    .TextCountOfHomePage = 4,
    .TextCountOfOtherPage = 6,
};

ImageMenu_t ImageMenu = {
    .NumOfPages = sizeof(HomePage) / sizeof(HomePage[0]),
    .ImageWidth = 32,
    .ImageHeight = 32,
    .Space = 32,
    .Page = HomePage,
};

SelectioneBar_t Bar;

void *Menu = &ImageMenu;

TextPage_t *StatusLEDSetting;
TextPage_t *ReverseSetting;
TextPage_t *RestartSetting;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vStateTimer;
TimerHandle_t vUpdateTimer;
TimerHandle_t vOLEDTimer;
void vStateTimerCallback(TimerHandle_t pxTimer);
void vUpdateTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);