#ifndef __LED_H
#define __LED_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define LOW  0
#define HIGH 1

typedef struct {
    char GPIOxPiny[32];
    uint8_t Mode;

    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
} LED_t;

void LED_Init(LED_t *self);

void LED_On(LED_t *self);
void LED_Off(LED_t *self);
void LED_Turn(LED_t *self);

#endif