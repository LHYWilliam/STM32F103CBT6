#include <math.h>

#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "RTC.h"

float invSqrt1(float x);
void  ICM42688_AHRS_CalculateGyroVariance(float *RawGyro, int Length,
                                          float *GyroVariance,
                                          float *GyroAverage);
void  ICM42688_AHRS_CalculateCalibratedAccGyro(ICM42688_t *Self);
void  ICM42688_AHRS_CalculateQ(ICM42688_t *Self);
void  ICM42688_AHRS_CalculateAngle(ICM42688_t *Self);

void ICM42688_AHRS_Init(ICM42688_t *Self) {
    Self->q[0]       = 1.0f;
    Self->q[1]       = 0.0f;
    Self->q[2]       = 0.0f;
    Self->q[3]       = 0.0f;
    Self->LastUpdate = RTC_Getus() / 100;
    Self->Now        = RTC_Getus() / 100;
}

void ICM42688_AHRS_GetRawAccGyro(ICM42688_t *Self, float *RawAccGyro) {
    RawAccGyro[0] = Self->RawAccGyro[0];
    RawAccGyro[1] = Self->RawAccGyro[1];
    RawAccGyro[2] = Self->RawAccGyro[2];
    RawAccGyro[3] = Self->RawAccGyro[3];
    RawAccGyro[4] = Self->RawAccGyro[4];
    RawAccGyro[5] = Self->RawAccGyro[5];
}

void ICM42688_AHRS_GetCalibratedAccGyro(ICM42688_t *Self,
                                        float      *CalibratedAccGyro) {
    CalibratedAccGyro[0] = Self->CalibratedAccGyro[0];
    CalibratedAccGyro[1] = Self->CalibratedAccGyro[1];
    CalibratedAccGyro[2] = Self->CalibratedAccGyro[2];
    CalibratedAccGyro[3] = Self->CalibratedAccGyro[3];
    CalibratedAccGyro[4] = Self->CalibratedAccGyro[4];
    CalibratedAccGyro[5] = Self->CalibratedAccGyro[5];
}

void ICM42688_AHRS_GetYawPitchRoll(ICM42688_t *Self, float *Angles) {
    Angles[0] = Self->Angles[0];
    Angles[1] = Self->Angles[1];
    Angles[2] = Self->Angles[2];
}

void ICM42688_AHRS_Update(ICM42688_t *Self) {
    ICM42688_GetRealAccGyro(Self, Self->RawAccGyro);
    ICM42688_AHRS_CalculateCalibratedAccGyro(Self);
    ICM42688_AHRS_CalculateQ(Self);
    ICM42688_AHRS_CalculateAngle(Self);
}

void ICM42688_AHRS_CalculateCalibratedAccGyro(ICM42688_t *Self) {
    static float GyroOffset[3] = {0};
    static int   CalCount      = 0;

    float GyroVariance[3], GyroAverage[3];
    ICM42688_AHRS_CalculateGyroVariance(&Self->RawAccGyro[3], 100, GyroVariance,
                                        GyroAverage);

    if (GyroVariance[0] < 0.02f && GyroVariance[1] < 0.02f &&
        GyroVariance[2] < 0.02f && CalCount >= 99) {
        GyroOffset[0] = GyroAverage[0];
        GyroOffset[1] = GyroAverage[1];
        GyroOffset[2] = GyroAverage[2];

        Self->XIntegral = 0;
        Self->YIntegral = 0;
        Self->ZIntegral = 0;

        CalCount = 0;
    } else if (CalCount < 100) {
        CalCount++;
    }

    Self->CalibratedAccGyro[0] = Self->RawAccGyro[0];
    Self->CalibratedAccGyro[1] = Self->RawAccGyro[1];
    Self->CalibratedAccGyro[2] = Self->RawAccGyro[2];
    Self->CalibratedAccGyro[3] = Self->RawAccGyro[3] - GyroOffset[0];
    Self->CalibratedAccGyro[4] = Self->RawAccGyro[4] - GyroOffset[1];
    Self->CalibratedAccGyro[5] = Self->RawAccGyro[5] - GyroOffset[2];
}

void ICM42688_AHRS_CalculateQ(ICM42688_t *Self) {
    float gx = Self->CalibratedAccGyro[3] * M_PI / 180;
    float gy = Self->CalibratedAccGyro[4] * M_PI / 180;
    float gz = Self->CalibratedAccGyro[5] * M_PI / 180;
    float ax = Self->CalibratedAccGyro[0];
    float ay = Self->CalibratedAccGyro[1];
    float az = Self->CalibratedAccGyro[2];
    float mx = Self->CalibratedAccGyro[6];
    float my = Self->CalibratedAccGyro[7];
    float mz = Self->CalibratedAccGyro[8];

    float norm;

    float vx, vy, vz;
    float ex, ey, ez, halfT;
    float tempq0, tempq1, tempq2, tempq3;

    float q0q0 = Self->q[0] * Self->q[0];
    float q0q1 = Self->q[0] * Self->q[1];
    float q0q2 = Self->q[0] * Self->q[2];
    float q0q3 = Self->q[0] * Self->q[3];
    float q1q1 = Self->q[1] * Self->q[1];
    float q1q2 = Self->q[1] * Self->q[2];
    float q1q3 = Self->q[1] * Self->q[3];
    float q2q2 = Self->q[2] * Self->q[2];
    float q2q3 = Self->q[2] * Self->q[3];
    float q3q3 = Self->q[3] * Self->q[3];

    Self->Now = RTC_Getus() / 100;
    if (Self->Now < Self->LastUpdate) {
        halfT = ((float)(Self->Now + (0xffff - Self->LastUpdate)) / 20000.0f);
    } else {
        halfT = ((float)(Self->Now - Self->LastUpdate) / 20000.0f);
    }
    Self->LastUpdate = Self->Now;

    norm = invSqrt1(ax * ax + ay * ay + az * az);
    ax   = ax * norm;
    ay   = ay * norm;
    az   = az * norm;

    norm = invSqrt1(mx * mx + my * my + mz * mz);
    mx   = mx * norm;
    my   = my * norm;
    mz   = mz * norm;

    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    if (ex != 0.0f && ey != 0.0f && ez != 0.0f) {
        Self->XIntegral = Self->XIntegral + ex * Self->Ki * halfT;
        Self->YIntegral = Self->YIntegral + ey * Self->Ki * halfT;
        Self->ZIntegral = Self->ZIntegral + ez * Self->Ki * halfT;

        gx = gx + Self->Kp * ex + Self->XIntegral;
        gy = gy + Self->Kp * ey + Self->YIntegral;
        gz = gz + Self->Kp * ez + Self->ZIntegral;
    }

    tempq0 = Self->q[0] +
             (-Self->q[1] * gx - Self->q[2] * gy - Self->q[3] * gz) * halfT;
    tempq1 = Self->q[1] +
             (Self->q[0] * gx + Self->q[2] * gz - Self->q[3] * gy) * halfT;
    tempq2 = Self->q[2] +
             (Self->q[0] * gy - Self->q[1] * gz + Self->q[3] * gx) * halfT;
    tempq3 = Self->q[3] +
             (Self->q[0] * gz + Self->q[1] * gy - Self->q[2] * gx) * halfT;

    norm       = invSqrt1(tempq0 * tempq0 + tempq1 * tempq1 + tempq2 * tempq2 +
                          tempq3 * tempq3);
    Self->q[0] = tempq0 * norm;
    Self->q[1] = tempq1 * norm;
    Self->q[2] = tempq2 * norm;
    Self->q[3] = tempq3 * norm;
}

void ICM42688_AHRS_CalculateAngle(ICM42688_t *Self) {
    Self->Angles[0] =
        -atan2(2 * Self->q[1] * Self->q[2] + 2 * Self->q[0] * Self->q[3],
               -2 * Self->q[2] * Self->q[2] - 2 * Self->q[3] * Self->q[3] + 1) *
        180 / M_PI;
    Self->Angles[1] =
        -asin(-2 * Self->q[1] * Self->q[3] + 2 * Self->q[0] * Self->q[2]) *
        180 / M_PI;
    Self->Angles[2] =
        atan2(2 * Self->q[2] * Self->q[3] + 2 * Self->q[0] * Self->q[1],
              -2 * Self->q[1] * Self->q[1] - 2 * Self->q[2] * Self->q[2] + 1) *
        180 / M_PI;
}

void ICM42688_AHRS_CalculateGyroVariance(float *RawGyro, int Length,
                                         float *GyroVariance,
                                         float *GyroAverage) {
    static double GyroSum[3];
    static int    GyroCount = 0;
    static double GyroBuffer[3][300];
    static double GyroSquareSum[3];
    static int    CalibrationFlag = 0;

    if (CalibrationFlag == 0) {
        for (uint8_t i = 0; i < 3; i++) {
            GyroSum[i] += RawGyro[i];
            GyroBuffer[i][GyroCount] = RawGyro[i];

            GyroAverage[i]  = 0;
            GyroVariance[i] = 100;
            GyroSquareSum[i] += RawGyro[i] * RawGyro[i];
        }

    } else {
        for (uint8_t i = 0; i < 3; i++) {
            GyroSum[i] -= GyroBuffer[i][GyroCount];
            GyroSquareSum[i] -=
                GyroBuffer[i][GyroCount] * GyroBuffer[i][GyroCount];

            GyroBuffer[i][GyroCount] = RawGyro[i];
            GyroSum[i] += GyroBuffer[i][GyroCount];
            GyroSquareSum[i] +=
                GyroBuffer[i][GyroCount] * GyroBuffer[i][GyroCount];
        }
    }

    GyroCount++;

    if (GyroCount >= Length) {
        GyroCount       = 0;
        CalibrationFlag = 1;
    }

    if (CalibrationFlag == 0) {
        return;
    }

    for (uint8_t i = 0; i < 3; i++) {
        GyroAverage[i] = (float)(GyroSum[i] / Length);
        GyroVariance[i] =
            (float)((GyroSquareSum[i] - GyroSum[i] * GyroSum[i] / Length) /
                    Length);
    }
}

float invSqrt1(float x) {
    float halfx = 0.5f * x;
    float y     = x;
    long  i     = *(long *)&y;
    i           = 0x5f3759df - (i >> 1);
    y           = *(float *)&i;
    y           = y * (1.5f - (halfx * y * y));

    return y;
}
