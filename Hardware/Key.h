#ifndef __KEY_H
#define __KEY_H

#include "GPIO.h"

#define LOW ((uint8_t)0)
#define HIGH ((uint8_t)1)

typedef struct {
    GPIO_t *gpio;
    uint8_t Mode;
} Key_t;

void Key_Init(Key_t *self);

uint8_t Key_Read(Key_t *self);

#endif