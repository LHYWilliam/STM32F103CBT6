#include "Key.h"
#include "Delay.h"

void Key_Init(Key_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = self->Mode ? GPIO_Mode_IPD : GPIO_Mode_IPU;
    self->IDR = GPIO_InitPin(&GPIO, self->GPIOxPiny);
}

uint8_t Key_Read(Key_t *self) {
    if (GPIO_ReadInput(self->IDR) == self->Mode) {
        Delay_ms(10);
        while (GPIO_ReadInput(self->IDR) == self->Mode)
            ;
        Delay_ms(10);
        return 1;
    }

    return 0;
}