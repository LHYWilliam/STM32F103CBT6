#ifndef PWMI_H
#define PWMI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "Capture.h"
#include "GPIO.h"
#include "TIM.h"

typedef struct {
    GPIO_t    *gpio;
    Capture_t *frequency;
    Capture_t *duty;
    TIM_t     *tim;
} PWMI_t;

void PWMI_Init(PWMI_t *self);

uint16_t PWMI_GetFrequency(PWMI_t *self);
uint16_t PWMI_GetDuty(PWMI_t *self);

#endif