#ifndef KEY_H
#define KEY_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef enum {
    KeyMode_Low,
    KeyMode_High,
} KeyMode;

typedef struct {
    GPIOxPiny_t GPIOxPiny;
    KeyMode     Mode;

    uint32_t IDR;
} Key_t;

void    Key_Init(Key_t *self);
uint8_t Key_Read(Key_t *self);

#endif