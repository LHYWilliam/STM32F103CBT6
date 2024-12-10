#include "Key.h"
#include "Delay.h"

void Key_Init(Key_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = self->Mode ? GPIO_Mode_IPD : GPIO_Mode_IPU;
    GPIO_InitPin(&GPIO, self->GPIOxPiny);

    self->GPIOx = GPIO.GPIOx;
    self->GPIO_Pin = GPIO.GPIO_Pin;
}

uint8_t Key_Read(Key_t *self) {
    if (GPIO_ReadInputDataBit(self->GPIOx, self->GPIO_Pin) == self->Mode) {
        Delay_ms(10);
        while (GPIO_ReadInputDataBit(self->GPIOx, self->GPIO_Pin) == self->Mode)
            ;
        Delay_ms(10);
        return 1;
    }

    return 0;
}