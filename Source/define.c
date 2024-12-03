#include "main.h"

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = LEDMode_High,
};

W25Q64_t W25Q64 = {
    .MOSI = A7,
    .MISO = A6,
    .SCK = A5,
    .CS = A4,
};

Serial_t Serial = {
    .TX = A2,
    .RX = A3,
    .USARTx = USART2,
    .BaudRate = 115200,
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
        .LED = B11,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED = B10,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED = B1,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED = B1,
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
    .ADC_Channel = "0 | 1 | 8 | 9",
    .GPIOxPiny = "A0 | A1 | B0 | B1",

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

#define BackHomePage                                                           \
    (TextPage_t) {                                                             \
        .Title = "<", .ClickCallback = BackHomePageCallbck,                    \
        .RotationCallback = TextMenuCursorCallback,                            \
    }

#define BackTextPage                                                           \
    (TextPage_t) {                                                             \
        .Title = "<", .ClickCallback = BackTextPageCallback,                   \
        .RotationCallback = TextMenuCursorCallback,                            \
    }

#define ChartPage(title)                                                       \
    (TextPage_t) {                                                             \
        .Title = title, .TitleX = 1, .TitleY = 64 / 4, .TitleWidth = 128 - 1,  \
        .TitleHeight = 64 / 2, .ShowCallback = ShowMQxPageCallback,            \
        .ClickCallback = EnterTextPageCallback,                                \
        .RotationCallback = TextMenuCursorCallback, .NumOfLowerPages = 1,      \
        .LowerPages = (TextPage_t[]) {                                         \
            BackTextPage,                                                      \
        }                                                                      \
    }

TextPage_t MQxChartPage[5] = {
    BackHomePage,     ChartPage("MQ2"),   ChartPage("MQ3"),
    ChartPage("MQ7"), ChartPage("MQ135"),
};

TextPage_t MonitorPage = {
    .Title = "异味检测与开窗系统",
    .ShowCallback = ShowMonitorPageCallback,
    .NumOfLowerPages = 5,
    .LowerPages = MQxChartPage,
};

TextPage_t SettingPage = {
    .Title = "Setting",
    .ShowCallback = ShowSettingPageCallback,
    .NumOfLowerPages = 7,
    .LowerPages =
        (TextPage_t[]){
            BackHomePage,
            (TextPage_t){
                .Title = "LED",
                .ClickCallback = SettingReverseCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
            (TextPage_t){
                .Title = "Reverse",
                .ClickCallback = SettingReverseCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
            (TextPage_t){
                .Title = "Test",
                .ClickCallback = SettingCursorToIncDecCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
            (TextPage_t){
                .Title = "Save",
                .ClickCallback = SettingSaveCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
            (TextPage_t){
                .Title = "Load",
                .ClickCallback = SettingLoadCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
            (TextPage_t){
                .Title = "Restart",
                .ClickCallback = RestartSettingCallback,
                .RotationCallback = TextMenuCursorCallback,
            },
        },
};

TextMenu_t TextMenu = {
    .Update = TextMenuUpdate_OneByOne,
    .TextCountOfHomePage = 4,
    .TextCountOfOtherPage = 6,
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

ImageMenu_t ImageMenu = {
    .Space = 32,
    .ImageWidth = 32,
    .ImageHeight = 32,
    .ShowCallback = ShowImageMenuCallback,
    .ClickCallback = ImagePageEnterTextPageCallback,
    .RotationCallback = ImageMenuCursorCallback,
    .NumOfPages = sizeof(HomePage) / sizeof(HomePage[0]),
    .Page = HomePage,
};

SelectioneBar_t Bar;

void *Menu = &ImageMenu;

TextPage_t *LEDSetting;
TextPage_t *ReverseSetting;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vUpdateTimer;
TimerHandle_t vOLEDTimer;
void vUpdateTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);