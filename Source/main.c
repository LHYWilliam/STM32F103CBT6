#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "Menu.h"
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

Key_t KeyUp = {
    .GPIOxPiny = B15,
    .Mode = LOW,
};

Key_t KeyDown = {
    .GPIOxPiny = A9,
    .Mode = LOW,
};

Key_t KeyConfirm = {
    .GPIOxPiny = A11,
    .Mode = LOW,
};

Key_t KeyCancel = {
    .GPIOxPiny = B13,
    .Mode = LOW,
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

TextMenu_t Menu = {
    .Page =
        &(TextPage_t){
            .Title = "Home Page",
            .NumOfLowerPages = 4,
            .LowerPages =
                (TextPage_t[]){
                    (TextPage_t){
                        .Title = "ADC",
                    },
                    (TextPage_t){
                        .Title = "...",
                    },
                    (TextPage_t){
                        .Title = "...",
                    },
                    (TextPage_t){
                        .Title = "...",
                    },
                },
        },
};
TextPage_t *HomePage;
TextPage_t *ADCPage;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vLEDTimer;
TimerHandle_t vSamplerTimer;
TimerHandle_t vMenuTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);
void vSamplerTimerCallback(TimerHandle_t pxTimer);
void vMenuTimerCallback(TimerHandle_t pxTimer);

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    LED_Init(&LED);
    Key_Init(&Key);

    Key_Init(&KeyUp);
    Key_Init(&KeyDown);
    Key_Init(&KeyConfirm);
    Key_Init(&KeyCancel);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    TextMenu_Init(&Menu);
    HomePage = Menu.Page;
    ADCPage = &Menu.Page->LowerPages[0];

    Sampler_Init(&Sampler);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    vSamplerTimer = xTimerCreate("vOLEDTimer", pdMS_TO_TICKS(100), pdTRUE,
                                 (void *)1, vSamplerTimerCallback);
    vMenuTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(100), pdTRUE,
                              (void *)2, vMenuTimerCallback);

    xTimerStart(vLEDTimer, 0);
    // xTimerStart(vSamplerTimer, 0);
    xTimerStart(vMenuTimer, 0);

    vTaskStartScheduler();
}