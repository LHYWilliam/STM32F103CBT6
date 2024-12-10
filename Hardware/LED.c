#include "LED.h"
#include "GPIO.h"

void LED_Init(LED_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;
    GPIO_InitPin(&GPIO, self->GPIOxPiny);

    self->ODR = GPIO_ODR(self->GPIOxPiny);

    LED_Off(self);
}

void LED_On(LED_t *self) { GPIO_Write(self->ODR, self->Mode); }

void LED_Off(LED_t *self) { GPIO_Write(self->ODR, !self->Mode); }

void LED_Toggle(LED_t *self) {
    GPIO_Write(self->ODR, !GPIO_ReadInput(self->ODR));
}