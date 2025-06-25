#include "Key.h"
#include "Delay.h"

void Key_Init(Key_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = Self->Mode ? GPIO_Mode_IPD : GPIO_Mode_IPU;
    Self->IDR = GPIO_InitPin(&GPIO, Self->GPIOxPiny);
}

uint8_t Key_Read(Key_t *Self) {
    if (GPIO_ReadInput(Self->IDR) == Self->Mode) {
        Delay_ms(10);
        while (GPIO_ReadInput(Self->IDR) == Self->Mode)
            ;
        Delay_ms(10);
        return 1;
    }

    return 0;
}