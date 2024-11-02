#include "LED.h"

void LED_Init(LED_t *self) { GPIO_Init_(self->gpio); }

void LED_On(LED_t *self) {
    GPIO_WriteBit(self->gpio->GPIOx, self->gpio->GPIO_Pin,
                  (BitAction)(self->Mode));
}

void LED_Off(LED_t *self) {
    GPIO_WriteBit(self->gpio->GPIOx, self->gpio->GPIO_Pin,
                  (BitAction)(!self->Mode));
}

void LED_Turn(LED_t *self) {
    uint8_t now = GPIO_ReadOutputDataBit(self->gpio->GPIOx, self->gpio->GPIO_Pin);
    GPIO_WriteBit(self->gpio->GPIOx, self->gpio->GPIO_Pin, (BitAction)(!now));
}