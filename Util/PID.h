#ifndef __PID_H
#define __PID_H

#include <stdint.h>

#define LIMIT(data, low, high)                                                 \
    do {                                                                       \
        data = data < low ? low : data;                                        \
        data = data > high ? high : data;                                      \
    } while (0)

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    uint16_t imax;

    float last_error, integrator;
    float last_derivative;
    uint32_t last_time;

    uint8_t NaN;
} PID_t;

void PID_Init(PID_t *self);

int16_t PID_Caculate(PID_t *self, float error);

#endif