#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "RTC.h"
#include "Sampler.h"
#include "Timer.h"

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
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .Continuous = ENABLE,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

    .DMAx = DMA1,
    .DMA_Channel = 1,
};

Timer_t Timer = {
    .TIMx = TIM2,
    .ms = 1000,
};

int main() {
    RTC_Init();

    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);
    Sampler_Init(&Sampler);
    Timer_Init(&Timer);

    for (;;) {
        OLED_ShowNum(&OLED, 1, 1, Sampler.Data[0], 6);
        OLED_ShowNum(&OLED, 2, 1, Sampler.Data[1], 6);

        if (Key_Read(&Key)) {
            LED_Turn(&LED);
        }
    }
}

void TIM2_IRQHandler() {
    if (TIM_GetITStatus(Timer.TIMx, TIM_IT_Update)) {
        LED_Turn(&LED);

        TIM_ClearITPendingBit(Timer.TIMx, TIM_IT_Update);
    }
}