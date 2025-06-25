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

void PWMI_Init(PWMI_t *Self);

uint16_t PWMI_GetFrequency(PWMI_t *Self);
uint16_t PWMI_GetDuty(PWMI_t *Self);

#endif