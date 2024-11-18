#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "Sampler.h"

LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = HIGH,
};

Key_t Key = {
    .GPIOxPiny = A0,
    .Mode = HIGH,
};

OLED_t OLED = {
    .SCL = B8,
    .SDA = B9,
    .I2C = ENABLE,
    .Width = 128,
    .Height = 64,
};

#define LENGTH 64
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1",
    .GPIOxPiny = A1,

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
    .Priority = 14,
};

TimerHandle_t vLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TimerHandle_t vOLEDTimer;
void vOLEDTimerCallback(TimerHandle_t pxTimer);

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);

    Sampler_Init(&Sampler);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    vOLEDTimer = xTimerCreate("vOLEDTimer", pdMS_TO_TICKS(100), pdTRUE,
                              (void *)1, vOLEDTimerCallback);

    xTimerStart(vLEDTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    vTaskStartScheduler();
}