#include "main.h"

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = LEDMode_High,
};

Key_t Key = {
    .GPIOxPiny = A0,
    .Mode = KeyMode_High,
};

Key_t KeyConfirm = {
    .GPIOxPiny = B15,
    .Mode = KeyMode_Low,
};

Encoder_t Encoder = {
    .GPIOxPiny = "A8 | A9",
    .TIMx = TIM1,
};

#define Threshold                                                              \
    Threshold = VoltageToADC(2.048), .Relaxation = VoltageToADC(0.128)

MQSensor_t MQSensor[4] = {
    (MQSensor_t){
        .LED = A5,
        .Mode = LEDMode_Low,
        .Threshold,
    },

    (MQSensor_t){
        .LED = A7,
        .Mode = LEDMode_Low,
        .Threshold,
    },

    (MQSensor_t){
        .LED = B1,
        .Mode = LEDMode_Low,
        .Threshold,
    },

    (MQSensor_t){
        .LED = B11,
        .Mode = LEDMode_Low,
        .Threshold,
    },
};

#define Samper_DataLength 4
uint16_t Data[Samper_DataLength];
Sampler_t Sampler = {
    .Data = Data,
    .Length = Samper_DataLength,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2 | 3 | 8",
    .GPIOxPiny = "A1 | A2 | A3 | B0",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

OLED_t OLED = {
    .SCL = B8,
    .SDA = B9,
    .I2C = ENABLE,
    .Width = 128,
    .Height = 64,
};

#define BackHomePage Title = "<", .ClickCallback = BackHomePageCallbck
#define BackTextPage Title = "<", .ClickCallback = BackTextPageCallback

#define ChartPage                                                              \
    TitleX = 1, .TitleY = 64 / 4, .TitleWidth = 128 - 1,                       \
    .TitleHeight = 64 / 2, .ClickCallback = EnterTextPageCallback,             \
    .RotationCallback = ThresholdCallback, .NumOfLowerPages = 1,               \
    .LowerPages = (TextPage_t[]) {                                             \
        (TextPage_t){                                                          \
            .BackTextPage,                                                     \
                                                                               \
        },                                                                     \
    }

extern void BackHomePageCallbck(void *pvParameters);
extern void EnterTextPageCallback(void *pvParameters);
extern void BackTextPageCallback(void *pvParameters);
extern void TextMenuCursorCallback(int16_t Encoder);
extern void ThresholdCallback(int16_t Encoder);
extern void SettingCallback(void *pvParameters);

extern void ImageMenuCursorCallback(int16_t Encoder);
extern void ImagePageEnterTextPageCallback(void *pvParameters);

TextPage_t MQxChartPage[5] = {
    (TextPage_t){
        .BackHomePage,
    },
    (TextPage_t){
        .Title = "MQ2",
        .ChartPage,
    },
    (TextPage_t){
        .Title = "MQ3",
        .ChartPage,
    },
    (TextPage_t){
        .Title = "MQ7",
        .ChartPage,
    },
    (TextPage_t){
        .Title = "MQ135",
        .ChartPage,
    },
};

TextPage_t MonitorPage = {
    .Title = "异味检测与开窗系统",
    .RotationCallback = TextMenuCursorCallback,
    .NumOfLowerPages = 5,
    .LowerPages = MQxChartPage,
};

TextPage_t SettingPage = {
    .Title = "Setting",
    .RotationCallback = TextMenuCursorCallback,
    .NumOfLowerPages = 4,
    .LowerPages =
        (TextPage_t[]){
            (TextPage_t){
                .BackHomePage,
            },
            (TextPage_t){
                .Title = "Status LED",
                .Setting = SET,
                .ClickCallback = SettingCallback,
            },
            (TextPage_t){
                .Title = "Reverse",
                .ClickCallback = SettingCallback,
            },
            (TextPage_t){
                .Title = "Restart",
                .ClickCallback = SettingCallback,
            },
        },
};

ImagePage_t HomePage[2] = {
    (ImagePage_t){
        .Title = "Montior",
        .Image = MenuImage[0],
        .TextPage = &MonitorPage,
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
    .Space = 32,
    .ImageWidth = 32,
    .ImageHeight = 32,
    .ClickCallback = ImagePageEnterTextPageCallback,
    .RotationCallback = ImageMenuCursorCallback,
    .NumOfPages = sizeof(HomePage) / sizeof(HomePage[0]),
    .Page = HomePage,
};

SelectioneBar_t Bar;

void *Menu = &ImageMenu;

TextPage_t *StatusLEDSetting;
TextPage_t *ReverseSetting;
TextPage_t *RestartSetting;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vUpdateTimer;
TimerHandle_t vOLEDTimer;
void vUpdateTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);