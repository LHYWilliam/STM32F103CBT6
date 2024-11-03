#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "RTC.h"

LED_t LED = {
    .GPIOxPiny = "B2",
    .Mode = HIGH,
};

Key_t Key = {
    .GPIOxPiny = "A0",
    .Mode = HIGH,
};

OLED_t OLED = {
    .SCL = "B8",
    .SDA = "B9",
};

int main() {
    RTC_Init();

    LED_Init(&LED);
    Key_Init(&Key);

    OLED_Init(&OLED);

    for (;;) {
        OLED_ShowNum(&OLED, 1, 1, RTC_time_s(), 6);
        if (Key_Read(&Key)) {
            LED_Turn(&LED);
        }
    }
}