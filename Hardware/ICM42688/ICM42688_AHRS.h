#ifndef ICM42688_AHRS_H
#define ICM42688_AHRS_H

#define M_PI (float)3.1415926535

#include "RTE_Components.h"
#include CMSIS_device_header

#include "ICM42688.h"

void ICM42688_AHRS_Init(ICM42688_t *Self);
void ICM42688_AHRS_Update(ICM42688_t *Self);
void ICM42688_AHRS_GetRawAccGyro(ICM42688_t *Self, float *Gyro);
void ICM42688_AHRS_GetYawPitchRoll(ICM42688_t *Self, float *Angles);

#endif
