#include "Delay.h"
#include "Key.h"

void Key_Init(Key_t *self) { GPIO_Init_(self->gpio); }

uint8_t Key_Read(Key_t *self) {
    uint8_t if_key = 0;
    if (GPIO_ReadInputDataBit(self->gpio->GPIOx, self->gpio->GPIO_Pin) ==
        self->Mode) {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(self->gpio->GPIOx, self->gpio->GPIO_Pin) ==
               self->Mode)
            ;
        Delay_ms(20);
        if_key = 1;
    }
    return if_key;
}