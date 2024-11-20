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

LED_t MQ3_LED = {
    .GPIOxPiny = B11,
    .Mode = LOW,
};

LED_t MQ135_LED = {
    .GPIOxPiny = B1,
    .Mode = LOW,
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

#define MQx_DataLength 128

int16_t MQ3_Index = -1;
uint16_t MQ3_Data[MQx_DataLength];

int16_t MQ135_Index = -1;
uint16_t MQ135_Data[MQx_DataLength];

#define LENGTH 2
uint16_t Data[LENGTH];
Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

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
            .NumOfLowerPages = 2,
            .LowerPages =
                (TextPage_t[]){
                    (TextPage_t){
                        .Title = "MQ-3",
                    },
                    (TextPage_t){
                        .Title = "MQ-135",
                    },
                },
        },
};
TextPage_t *HomePage;
TextPage_t *MQ3Page;
TextPage_t *MQ135Page;

TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

TimerHandle_t vLEDTimer;
TimerHandle_t vOLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);

int main() {
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    LED_Init(&LED);
    Key_Init(&Key);

    LED_Init(&MQ3_LED);
    LED_Init(&MQ135_LED);
    Key_Init(&KeyUp);
    Key_Init(&KeyDown);
    Key_Init(&KeyConfirm);
    Key_Init(&KeyCancel);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    TextMenu_Init(&Menu);
    HomePage = Menu.Page;
    MQ3Page = &Menu.Page->LowerPages[0];
    MQ135Page = &Menu.Page->LowerPages[1];

    Sampler_Init(&Sampler);

    xTaskCreate(vMenuKeyTaskCode, "vMenuKeyTask", 128, NULL, 1,
                &xMenuKeyTaskHandle);

    vLEDTimer = xTimerCreate("vLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    vOLEDTimer = xTimerCreate("vMenuTimer", pdMS_TO_TICKS(100), pdTRUE,
                              (void *)2, vOLEDTimerCallback);

    xTimerStart(vLEDTimer, 0);
    xTimerStart(vOLEDTimer, 0);

    vTaskStartScheduler();
}