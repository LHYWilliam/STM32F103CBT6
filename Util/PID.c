#include "RTE_Components.h"
#include CMSIS_device_header

#include "PID.h"
#include "RTC.h"

float RC = 1 / (2 * 3.14 * 20);

void PID_Init(PID_t *Self) {
    Self->NaN            = RESET;
    Self->LastTime       = 0;
    Self->LastError      = 0.;
    Self->Integrator     = 0.;
    Self->LastDerivative = 0.;
}

int16_t PID_Caculate(PID_t *Self, float Error) {
    float    output = 0;
    uint32_t now    = RTC_Getms();
    float    dt     = (float)(now - Self->LastTime) / 1000;

    if (Self->LastTime == 0 || dt > 1) {
        Self->Integrator = dt = 0;
        Self->NaN             = SET;
    }
    Self->LastTime = now;

    output += Error * Self->Kp;

    if (Self->Kd && dt) {
        float derivative;
        if (Self->NaN == SET) {
            derivative           = 0;
            Self->LastDerivative = 0;
            Self->NaN            = RESET;
        } else {
            derivative = (Error - Self->LastError) / dt;
        }

        derivative = Self->LastDerivative +
                     (dt / (RC + dt)) * (derivative - Self->LastDerivative);
        Self->LastError      = Error;
        Self->LastDerivative = derivative;

        output += Self->Kd * derivative;
    }

    if (Self->Ki && dt) {
        Self->Integrator += Error * Self->Ki * dt;
        LIMIT(Self->Integrator, -Self->imax, Self->imax);

        output += Self->Integrator;
    }

    return (int32_t)output;
}