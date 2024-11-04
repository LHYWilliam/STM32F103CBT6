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

#define LENGTH 128
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

    OLED_ShowString(&OLED, 2, 2, ".   V");
    for (;;) {
        OLED_ShowNum(&OLED, 1, 1, Sampler.Index, 3);

        uint16_t temp = Sampler.Data[Sampler.Index];
        OLED_ShowNum(&OLED, 2, 1, (uint16_t)(temp * 3.3 / 4095.), 1);
        OLED_ShowNum(&OLED, 2, 3, (uint16_t)(temp * 3.3 / 4095. * 1000.) % 1000,
                     3);

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

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(Sampler.TIMx, TIM_IT_Update)) {
        Sampler.Index = (Sampler.Index + 1) % Sampler.Length;

        TIM_ClearITPendingBit(Sampler.TIMx, TIM_IT_Update);
    }
}