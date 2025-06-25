#include "ICM42688.h"
#include "Delay.h"
#include "ICM42688_AHRS.h"
#include "ICM42688_SPI.h"

void ICM42688_ConfigReg(ICM42688_t *Self);

void ICM42688_Init(ICM42688_t *Self) {
    if (Self->SPIx) {
        ICM42688_HWSPI_Init(Self);
    }

    ICM42688_ConfigReg(Self);

    ICM42688_AHRS_Init(Self);
}

void ICM42688_GetAccGyro(ICM42688_t *Self, float *AccGyro) {
    uint8_t Buffer[12];
    ICM42688_HWSPI_ReadRegs(Self, ICM42688_ACCEL_DATA_X1, Buffer, 12);

    AccGyro[0] = (float)((int16_t)(((uint16_t)Buffer[0] << 8) | Buffer[1]) *
                         Self->AccSensitivity);
    AccGyro[1] = (float)((int16_t)(((uint16_t)Buffer[2] << 8) | Buffer[3]) *
                         Self->AccSensitivity);
    AccGyro[2] = (float)((int16_t)(((uint16_t)Buffer[4] << 8) | Buffer[5]) *
                         Self->AccSensitivity);
    AccGyro[3] = (float)((int16_t)(((uint16_t)Buffer[6] << 8) | Buffer[7]) *
                         Self->GyroSensitivity);
    AccGyro[4] = (float)((int16_t)(((uint16_t)Buffer[8] << 8) | Buffer[9]) *
                         Self->GyroSensitivity);
    AccGyro[5] = (float)((int16_t)(((uint16_t)Buffer[10] << 8) | Buffer[11]) *
                         Self->GyroSensitivity);
}

void ICM42688_GetTemperature(ICM42688_t *Self, int16_t *Temperature) {
    uint8_t Buffer[2];
    ICM42688_HWSPI_ReadRegs(Self, ICM42688_TEMP_DATA1, Buffer, 2);

    *Temperature =
        (int16_t)(((int16_t)((Buffer[0] << 8) | Buffer[1])) / 132.48 + 25);
}

void ICM42688_GetAres(ICM42688_t *Self, uint8_t AccScale) {
    switch (AccScale) {
    case AFS_2G:
        Self->AccSensitivity = 2000 / 32768.0f;
        break;
    case AFS_4G:
        Self->AccSensitivity = 4000 / 32768.0f;
        break;
    case AFS_8G:
        Self->AccSensitivity = 8000 / 32768.0f;
        break;
    case AFS_16G:
        Self->AccSensitivity = 16000 / 32768.0f;
        break;
    }
}

void ICM42688_GetGres(ICM42688_t *Self, uint8_t GyroScale) {
    switch (GyroScale) {
    case GFS_15_125DPS:
        Self->GyroSensitivity = 15.125f / 32768.0f;
        break;
    case GFS_31_25DPS:
        Self->GyroSensitivity = 31.25f / 32768.0f;
        break;
    case GFS_62_5DPS:
        Self->GyroSensitivity = 62.5f / 32768.0f;
        break;
    case GFS_125DPS:
        Self->GyroSensitivity = 125.0f / 32768.0f;
        break;
    case GFS_250DPS:
        Self->GyroSensitivity = 250.0f / 32768.0f;
        break;
    case GFS_500DPS:
        Self->GyroSensitivity = 500.0f / 32768.0f;
        break;
    case GFS_1000DPS:
        Self->GyroSensitivity = 1000.0f / 32768.0f;
        break;
    case GFS_2000DPS:
        Self->GyroSensitivity = 2000.0f / 32768.0f;
        break;
    }
}

void ICM42688_ConfigReg(ICM42688_t *Self) {
    uint8_t Value = 0;

    Value = ICM42688_HWSPI_ReadReg(Self, ICM42688_WHO_AM_I);
    ICM42688_HWSPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0);
    ICM42688_HWSPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x01);
    Delay_ms(100);

    if (Value == ICM42688_ID) {
        ICM42688_GetAres(Self, AFS_4G);
        ICM42688_HWSPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);

        Value = (AFS_4G << 5);
        Value |= (AODR_100Hz);
        ICM42688_HWSPI_WriteReg(Self, ICM42688_ACCEL_CONFIG0, Value);

        ICM42688_GetGres(Self, GFS_1000DPS);
        ICM42688_HWSPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);

        Value = (GFS_1000DPS << 5);
        Value |= (GODR_100Hz);
        ICM42688_HWSPI_WriteReg(Self, ICM42688_GYRO_CONFIG0, Value);

        ICM42688_HWSPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);
        Value = ICM42688_HWSPI_ReadReg(Self, ICM42688_PWR_MGMT0);
        Value &= ~(1 << 5);
        Value |= ((3) << 2);
        Value |= (3);
        ICM42688_HWSPI_WriteReg(Self, ICM42688_PWR_MGMT0, Value);
        Delay_ms(1);
    }
}
