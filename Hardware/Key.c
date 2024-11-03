#include <string.h>

#include "Delay.h"
#include "GPIO.h"
#include "Key.h"

void Key_Init(Key_t *self) {
    GPIO_t gpio = {
        .Mode = self->Mode ? GPIO_Mode_IPD : GPIO_Mode_IPU,
    };
    strcpy(gpio.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init_(&gpio);

    self->GPIOx = gpio.GPIOx;
    self->GPIO_Pin = gpio.GPIO_Pin;
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