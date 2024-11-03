#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
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

Motor_t Motor = {
    .PWM = A8,
    .IN1 = A9,
    .IN2 = A10,
    .TIMx = TIM1,
    .Channel = 1,
    .TIM_Init = ENABLE,
};

Sampler_t Sampler = {
    .ADCx = ADC1,
    .Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",
    .Continuous = DISABLE,
};

int main() {
    RTC_Init();

    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);
    // Motor_Init(&Motor);
    Sampler_Init_(&Sampler);

    for (;;) {
        OLED_ShowNum(&OLED, 1, 1, Sampler_Get(&Sampler, 1), 6);
        OLED_ShowNum(&OLED, 2, 1, Sampler_Get(&Sampler, 2), 6);
        if (Key_Read(&Key)) {
            LED_Turn(&LED);
        }
    }
}