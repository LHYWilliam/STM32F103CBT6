#include "Key.h"
#include "Delay.h"
#include "GPIO.h"

void Key_Init(Key_t *self) {
    GPIO_t GPIO = {
        .Mode = self->Mode ? GPIO_Mode_IPD : GPIO_Mode_IPU,
    };
    strcpy(GPIO.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init_(&GPIO);

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