#include "LED.h"

void LED_Init(LED_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;
    Self->ODR = GPIO_InitPin(&GPIO, Self->GPIOxPiny);

    LED_Off(Self);
}

void LED_On(LED_t *Self) { GPIO_Write(Self->ODR, Self->Mode); }

void LED_Off(LED_t *Self) { GPIO_Write(Self->ODR, !Self->Mode); }

void LED_Toggle(LED_t *Self) { GPIO_Toggle(Self->ODR); }