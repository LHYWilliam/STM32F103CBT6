#ifndef __LED_H
#define __LED_H

#include "GPIO.h"

#define LOW ((uint8_t)0)
#define HIGH ((uint8_t)01)

typedef struct {
    GPIO_t *gpio;
    uint8_t Mode;
} LED_t;

void LED_Init(LED_t *self);

void LED_On(LED_t *self);
void LED_Off(LED_t *self);
void LED_Turn(LED_t *self);

#endif