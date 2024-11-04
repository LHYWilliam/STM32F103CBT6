#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
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

#define LENGTH 1
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1",
    .GPIOxPiny = "A1",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
};

Timer_t Timer = {
    .TIMx = TIM2,
    .ms = 1000,
    .Interrupt = ENABLE,
    .Priority = 15,
};

int main() {
    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);
    Sampler_Init(&Sampler);
    Timer_Init(&Timer);

    OLED_ShowString(&OLED, 1, 2, ".   V");
    for (;;) {
        uint16_t data1 = Sampler.Data[0];
        OLED_ShowNum(&OLED, 1, 1, (uint16_t)(data1 * 3.3 / 4095.), 1);
        OLED_ShowNum(&OLED, 1, 3,
                     (uint16_t)(data1 * 3.3 / 4095. * 1000.) % 1000, 3);

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