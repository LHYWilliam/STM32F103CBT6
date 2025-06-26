#include "main.h"

// #define ADCToVoltage(ADC)     ((ADC) / 4095.0 * 3.3)
// #define VoltageToADC(Voltage) (uint32_t)((Voltage) / 3.3 * 4095.0)

// #define Threshold \
//     Threshold = VoltageToADC(2.048), .Relaxation = VoltageToADC(0.128)

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode      = LEDMode_High,
};

Key_t Key = {
    .GPIOxPiny = A0,
    .Mode      = KeyMode_High,
};

Serial_t Serial = {
    .TX       = A9,
    .RX       = A10,
    .USARTx   = USART1,
    .BaudRate = 115200,
    .Default  = ENABLE,
};

// Encoder_t Encoder = {
//     .GPIOxPiny = {B6, B7},
//     .TIMx      = TIM4,
//     .Channel   = {0, 1},
// };

// #define Samper_DataLength 4
// uint16_t  Data[Samper_DataLength];
// Sampler_t Sampler = {
//     .Data   = Data,
//     .Length = Samper_DataLength,

//     .ADCx        = ADC1,
//     .ADC_Channel = {9, 8, 1, 0},
//     .GPIOxPiny   = {B1, B0, A1, A0},

//     .DMAx        = DMA1,
//     .DMA_Channel = 1,

//     .TIMx     = TIM3,
//     .Hz       = 10,
//     .Priority = 14,
// };

// OLED_t OLED = {
//     .SCL    = B12,
//     .SDA    = B13,
//     .I2C    = ENABLE,
//     .Width  = 128,
//     .Height = 64,
// };

// LCD_t LCD = {
//     .SCL       = B12,
//     .SDA       = B13,
//     .RES       = B14,
//     .DC        = B15,
//     .CS        = B10,
//     .BL        = B11,
//     .Width     = 160,
//     .Height    = 128,
//     .Direction = 2,
//     .BackColor = 0xFFFF,
// };

// W25Q64_t W25Q64 = {
//     .MOSI = A7,
//     .MISO = A6,
//     .SCK  = A5,
//     .CS   = A4,
// };

// MQSensor_t MQSensor[4] = {
//     (MQSensor_t){
//         .LED  = B11,
//         .Mode = LEDMode_Low,
//         .Threshold,
//     },
//     (MQSensor_t){
//         .LED  = B10,
//         .Mode = LEDMode_Low,
//         .Threshold,
//     },
//     (MQSensor_t){
//         .LED  = C14,
//         .Mode = LEDMode_Low,
//         .Threshold,
//     },
//     (MQSensor_t){
//         .LED  = C13,
//         .Mode = LEDMode_Low,
//         .Threshold,
//     },
// };

ICM42688_t ICM42688 = {
    .SCLK = B13,
    .MISO = B14,
    .MOSI = B15,
    .CS   = A2,
    .SPIx = SPI2,
    .Kp   = 0.5f,
    .Ki   = 0.001f,
};

TaskHandle_t xMainTaskHandle;

void vMainTaskCode(void *pvParameters);

TimerHandle_t vLEDTimer;

void vLEDTimerCallback(TimerHandle_t pxTimer);
