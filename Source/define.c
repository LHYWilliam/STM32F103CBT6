#include "main.h"

#define Threshold                                                              \
    Threshold = VoltageToADC(2.048), .Relaxation = VoltageToADC(0.128)

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

#define SettingReversePage(title)                                              \
    (TextPage_t) {                                                             \
        .Title = title, .ClickCallback = SettingReverseCallback,               \
        .RotationCallback = TextMenuCursorCallback,                            \
    }

#define SettingIncDecPage(title)                                               \
    (TextPage_t) {                                                             \
        .Title = title, .ClickCallback = SettingCursorToIncDecCallback,        \
        .RotationCallback = TextMenuCursorCallback,                            \
    }

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
    .GPIOxPiny = B5,
    .Mode = KeyMode_Low,
};

Encoder_t Encoder = {
    .GPIOxPiny = {B6, B7},
    .TIMx = TIM4,
    .Channel = {0, 1},
};

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
        .LED = C14,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED = C13,
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
    .ADC_Channel = {9, 8, 1, 0},
    .GPIOxPiny = {B1, B0, A1, A0},

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

OLED_t OLED = {
    .SCL = B12,
    .SDA = B13,
    .I2C = ENABLE,
    .Width = 128,
    .Height = 64,
};

LCD_t LCD = {
    .SCL = B12,
    .SDA = B13,
    .RES = B14,
    .DC = B15,
    .CS = B10,
    .BL = B11,
    .Width = 160,
    .Height = 128,
    .Direction = 2,
    .BackColor = 0xFFFF,
};

TextPage_t MonitorPage = {
    .Title = "异味检测与开窗系统",
    .ShowCallback = ShowMonitorPageCallback,
    .NumOfLowerPages = 5,
    .LowerPages =
        (TextPage_t[]){
            BackHomePage,
            ChartPage("MQ2"),
            ChartPage("MQ3"),
            ChartPage("MQ7"),
            ChartPage("MQ135"),
        },
};

TextPage_t SettingPage = {
    .Title = "Setting",
    .ShowCallback = ShowSettingPageCallback,
    .NumOfLowerPages = 7,
    .LowerPages =
        (TextPage_t[]){
            BackHomePage,
            SettingReversePage("LED"),
            SettingReversePage("Reverse"),
            SettingIncDecPage("Test"),
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

ImageMenu_t ImageMenu = {
    .Space = 32,
    .ImageWidth = 32,
    .ImageHeight = 32,
    .ShowCallback = ShowImageMenuCallback,
    .ClickCallback = ImagePageEnterTextPageCallback,
    .RotationCallback = ImageMenuCursorCallback,
    .NumOfPages = 2,
    .Page =
        (ImagePage_t[]){
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
        },
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