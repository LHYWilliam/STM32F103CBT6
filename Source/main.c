#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "RTC.h"
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
};

#define LENGTH 2
uint16_t ADC_Value[LENGTH];

Sampler_t Sampler = {
    .Data = ADC_Value,
    .Length = LENGTH,
    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",
    .Continuous = ENABLE,
    .DMAx = DMA1,
    .DMA_Channel = 1,
};

int main() {
    RTC_Init();

    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);
    Sampler_Init_(&Sampler);

    for (;;) {
        OLED_ShowNum(&OLED, 1, 1, Sampler.Data[0], 6);
        OLED_ShowNum(&OLED, 2, 1, Sampler.Data[1], 6);

        if (Key_Read(&Key)) {
            LED_Turn(&LED);
        }
    }
}