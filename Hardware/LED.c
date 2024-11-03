#include <string.h>

#include "GPIO.h"
#include "LED.h"

void LED_Init(LED_t *self) {
    GPIO_t gpio = {
        .Mode = GPIO_Mode_Out_PP,
    };
    strcpy(gpio.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init_(&gpio);

    self->GPIOx = gpio.GPIOx;
    self->GPIO_Pin = gpio.GPIO_Pin;
}

void LED_On(LED_t *self) {
    GPIO_WriteBit(self->GPIOx, self->GPIO_Pin, (BitAction)(self->Mode));
}

void LED_Off(LED_t *self) {
    GPIO_WriteBit(self->GPIOx, self->GPIO_Pin, (BitAction)(!self->Mode));
}

void LED_Turn(LED_t *self) {
    GPIO_WriteBit(
        self->GPIOx, self->GPIO_Pin,
        (BitAction)(!GPIO_ReadOutputDataBit(self->GPIOx, self->GPIO_Pin)));
}