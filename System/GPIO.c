#include <string.h>

#include "GPIO.h"

void GPIO_Init_(GPIO_t *self) {
    char *temp = self->GPIOxPiny;
    do {
        self->GPIOx = GPIOx(temp);
        self->GPIO_Pin = GPIO_Pinx(temp);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx(temp), ENABLE);

        GPIO_InitTypeDef GPIO_InitStruct = {
            .GPIO_Pin = self->GPIO_Pin,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = self->Mode,
        };
        GPIO_Init(self->GPIOx, &GPIO_InitStruct);
    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2));
}