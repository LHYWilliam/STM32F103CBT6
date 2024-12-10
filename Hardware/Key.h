#ifndef KEY_H
#define KEY_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef enum {
    KeyMode_Low,
    KeyMode_High,
} KeyMode;

typedef struct {
    char GPIOxPiny[32];
    KeyMode Mode;

    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
} Key_t;

void Key_Init(Key_t *self);
uint8_t Key_Read(Key_t *self);

#endif