#ifndef PID_H
#define PID_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define LIMIT(data, low, high)            \
    do {                                  \
        data = data < low ? low : data;   \
        data = data > high ? high : data; \
    } while (0)

typedef struct {
    float    Kp;
    float    Ki;
    float    Kd;
    uint16_t imax;

    float    LastError;
    float    Integrator;
    float    LastDerivative;
    uint32_t LastTime;

    uint8_t NaN;
} PID_t;

void    PID_Init(PID_t *Self);
int16_t PID_Caculate(PID_t *Self, float Error);

#endif