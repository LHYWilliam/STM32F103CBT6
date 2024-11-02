#include "RTE_Components.h"
#include CMSIS_device_header

#include "PID.h"
#include "RTC.h"

float RC = 1 / (2 * 3.14 * 20);

void PID_Init(PID_t *self) {
    self->NaN = RESET;
    self->last_time = 0;
    self->last_error = 0.;
    self->integrator = 0.;
    self->last_derivative = 0.;
}

int16_t PID_Caculate(PID_t *self, float error) {
    float output = 0;
    uint32_t now = RTC_time_ms();
    float dt = (float)(now - self->last_time) / 1000;

    if (self->last_time == 0 || dt > 1) {
        self->integrator = dt = 0;
        self->NaN = SET;
    }
    self->last_time = now;

    output += error * self->Kp;

    if (self->Kd && dt) {
        float derivative;
        if (self->NaN == SET) {
            derivative = 0;
            self->last_derivative = 0;
            self->NaN = RESET;
        } else {
            derivative = (error - self->last_error) / dt;
        }

        derivative = self->last_derivative +
                     (dt / (RC + dt)) * (derivative - self->last_derivative);
        self->last_error = error;
        self->last_derivative = derivative;

        output += self->Kd * derivative;
    }

    if (self->Ki && dt) {
        self->integrator += error * self->Ki * dt;
        LIMIT(self->integrator, -self->imax, self->imax);

        output += self->integrator;
    }

    return (int32_t)output;
}