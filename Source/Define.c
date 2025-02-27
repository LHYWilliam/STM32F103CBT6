#include "main.h"

#define Threshold \
    Threshold = VoltageToADC(2.048), .Relaxation = VoltageToADC(0.128)

#define TextPage_BackHome(title)                                         \
    (TextPage_t) {                                                       \
        .Title = title, .ClickCallback = TextPage_BackImageMenuCallback, \
        .RotationCallback = TextPage_CursorCallback,                     \
    }

#define TextPage_Back(title)                                    \
    (TextPage_t) {                                              \
        .Title = title, .ClickCallback = TextPage_BackCallback, \
        .RotationCallback = TextPage_CursorCallback,            \
    }

#define TextPage_Chart(title)                                                 \
    (TextPage_t) {                                                            \
        .Title = title, .TitleX = 1, .TitleY = 64 / 4, .TitleWidth = 128 - 1, \
        .TitleHeight = 64 / 2, .ShowCallback = TextPage_ShowMQxCallback,      \
        .ClickCallback    = TextPage_EnterCallback,                           \
        .RotationCallback = TextPage_CursorCallback, .NumOfLowerPages = 1,    \
        .LowerPages = (TextPage_t[]) {                                        \
            TextPage_Back("<"),                                               \
        }                                                                     \
    }

#define TextPage_ReverseSetting(title)                                    \
    (TextPage_t) {                                                        \
        .Title = title, .ClickCallback = TextPage_SettingReverseCallback, \
        .RotationCallback = TextPage_CursorCallback,                      \
    }

#define TextPage_EncoderSetting(title)                                   \
    (TextPage_t) {                                                       \
        .Title            = title,                                       \
        .ClickCallback    = TextPage_SettingCursorSwitchEncoderCallback, \
        .RotationCallback = TextPage_CursorCallback,                     \
    }

#define TextPage_ConfirmDialog(title, click)                                 \
    (TextPage_t) {                                                           \
        .Title = title, .TitleX = 64, .TitleY = 32, .TitleWidth = 1,         \
        .TitleHeight = 1, .ShowCallback = TextPage_ShowDialogCallback,       \
        .UpdateCallback = TextPage_UpdateDialogCallback,                     \
        .ClickCallback = click, .RotationCallback = TextPage_CursorCallback, \
        .NumOfLowerPages = 1,                                                \
        .LowerPages      = (TextPage_t[]){                                   \
            (TextPage_t){                                               \
                     .Title         = "back",                                \
                     .X             = 128 / 2 - 6 * 4 / 2,                   \
                     .Y             = 64 / 2,                                \
                     .ClickCallback = TextPage_BackCallback,                 \
            },                                                          \
        },                                                              \
    }

#define TextPage_RestartSetting(title)                                    \
    (TextPage_t) {                                                        \
        .Title = title, .ClickCallback = TextPage_SettingRestartCallback, \
        .RotationCallback = TextPage_CursorCallback,                      \
    }

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode      = LEDMode_High,
};

W25Q64_t W25Q64 = {
    .MOSI = A7,
    .MISO = A6,
    .SCK  = A5,
    .CS   = A4,
};

Serial_t Serial = {
    .TX       = A2,
    .RX       = A3,
    .USARTx   = USART2,
    .BaudRate = 115200,
};

Key_t KeyConfirm = {
    .GPIOxPiny = B5,
    .Mode      = KeyMode_Low,
};

Encoder_t Encoder = {
    .GPIOxPiny = {B6, B7},
    .TIMx      = TIM4,
    .Channel   = {0, 1},
};

MQSensor_t MQSensor[4] = {
    (MQSensor_t){
        .LED  = B11,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED  = B10,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED  = C14,
        .Mode = LEDMode_Low,
        .Threshold,
    },
    (MQSensor_t){
        .LED  = C13,
        .Mode = LEDMode_Low,
        .Threshold,
    },
};

#define Samper_DataLength 4
uint16_t  Data[Samper_DataLength];
Sampler_t Sampler = {
    .Data   = Data,
    .Length = Samper_DataLength,

    .ADCx        = ADC1,
    .ADC_Channel = {9, 8, 1, 0},
    .GPIOxPiny   = {B1, B0, A1, A0},

    .DMAx        = DMA1,
    .DMA_Channel = 1,

    .TIMx     = TIM3,
    .Hz       = 10,
    .Priority = 14,
};

OLED_t OLED = {
    .SCL    = B12,
    .SDA    = B13,
    .I2C    = ENABLE,
    .Width  = 128,
    .Height = 64,
};

LCD_t LCD = {
    .SCL       = B12,
    .SDA       = B13,
    .RES       = B14,
    .DC        = B15,
    .CS        = B10,
    .BL        = B11,
    .Width     = 160,
    .Height    = 128,
    .Direction = 2,
    .BackColor = 0xFFFF,
};

TextPage_t MonitorPage = {
    .Title           = "异味检测与开窗系统",
    .ShowCallback    = TextPage_ShowMonitorCallback,
    .UpdateCallback  = TextPage_UpdateCallback,
    .NumOfLowerPages = 5,
    .LowerPages =
        (TextPage_t[]){
            TextPage_BackHome("<"),
            TextPage_Chart("MQ2"),
            TextPage_Chart("MQ3"),
            TextPage_Chart("MQ7"),
            TextPage_Chart("MQ135"),
        },
};

TextPage_t SettingPage = {
    .Title           = "Setting",
    .ShowCallback    = TextPage_ShowSettingCallback,
    .UpdateCallback  = TextPage_UpdateCallback,
    .NumOfLowerPages = 7,
    .LowerPages =
        (TextPage_t[]){
            TextPage_BackHome("<"),
            TextPage_ReverseSetting("LED"),
            TextPage_ReverseSetting("Reverse"),
            TextPage_EncoderSetting("Encoder"),
            TextPage_ConfirmDialog("Save", TextPage_SettingSaveCallback),
            TextPage_ConfirmDialog("Load", TextPage_SettingLoadCallback),
            TextPage_RestartSetting("Restart"),
        },
};

TextMenu_t TextMenu;

ImageMenu_t ImageMenu = {
    .Space            = 32,
    .ImageWidth       = 32,
    .ImageHeight      = 32,
    .ShowCallback     = ImagePage_ShowCallback,
    .ClickCallback    = ImagePage_EnterTextPageCallback,
    .RotationCallback = ImagePage_CursorCallback,
    .NumOfPages       = 2,
    .Page =
        (ImagePage_t[]){
            (ImagePage_t){
                .Title    = "Montior",
                .Image    = MenuImage[0],
                .TextPage = &MonitorPage,
            },
            (ImagePage_t){
                .Title    = "Setting",
                .Image    = MenuImage[2],
                .TextPage = &SettingPage,
            },
        },
};

SelectioneBar_t Bar;

void *Menu = &ImageMenu;

TextPage_t *LEDSetting;
TextPage_t *ReverseSetting;

TaskHandle_t xMenuKeyTaskHandle;
void         vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vUpdateTimer;
TimerHandle_t vOLEDTimer;
void          vUpdateTimerCallback(TimerHandle_t pxTimer);
void          vOLEDTimerCallback(TimerHandle_t pxTimer);