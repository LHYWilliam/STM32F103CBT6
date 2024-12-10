#ifndef LED_H
#define LED_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef enum {
    LEDMode_Low,
    LEDMode_High,
} LEDMode;

typedef struct {
    char GPIOxPiny[32];
    LEDMode Mode;

    uint32_t ODR;
} LED_t;

void LED_Init(LED_t *self);
void LED_On(LED_t *self);
void LED_Off(LED_t *self);
void LED_Toggle(LED_t *self);

#endif