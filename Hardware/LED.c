#include "LED.h"
#include "GPIO.h"

void LED_Init(LED_t *self) {
    GPIO_t GPIO = {
        .Mode = GPIO_Mode_Out_PP,
    };
    GPIO_InitPin(GPIO, self->GPIOxPiny);

    self->GPIOx = GPIO.GPIOx;
    self->GPIO_Pin = GPIO.GPIO_Pin;

    LED_Off(self);
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