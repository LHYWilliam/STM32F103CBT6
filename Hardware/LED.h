#ifndef LED_H
#define LED_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef enum {
    LEDMode_Low,
    LEDMode_High,
} LEDMode;

typedef struct {
    GPIOxPiny_t GPIOxPiny;
    LEDMode     Mode;

    uint32_t ODR;
} LED_t;

void LED_Init(LED_t *Self);
void LED_On(LED_t *Self);
void LED_Off(LED_t *Self);
void LED_Toggle(LED_t *Self);

#endif