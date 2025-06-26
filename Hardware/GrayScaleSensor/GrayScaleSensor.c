#include "Delay.h"

#include "GrayScaleSensor.h"
#include "GrayScaleSensor_I2C.h"

ErrorStatus GrayScaleSensor_ScanAddress(GrayScaleSensor_t *Self);

ErrorStatus GrayScaleSensor_Init(GrayScaleSensor_t *Self) {
    Delay_ms(100);

    GraySacleSensor_SWI2C_Init(Self);

    if (!GrayScaleSensor_ScanAddress(Self)) {
        return ERROR;
    }

    Delay_ms(100);

    if (Self->Mode == GrayScaleSensorMode_Analog) {
        uint8_t Temp[8];
        GraySacleSensor_SWI2C_SingedAddrReadBytes(Self, Self->Addr << 1,
                                                  GW_GRAY_ANALOG_MODE, Temp, 8);
    }

#if GW_READ_DIGITAL_DATA
    uint8_t gray_sensor[8];
    uint8_t digital_data;

    sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);

    sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data);
    while (1) {

        sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data);

        if (digital_data == 0xF0) {
        }

        if (GET_NTH_BIT(digital_data, 2)) {
        }

        SEP_ALL_BIT8(digital_data, gray_sensor[0], gray_sensor[1],
                     gray_sensor[2], gray_sensor[3], gray_sensor[4],
                     gray_sensor[5], gray_sensor[6], gray_sensor[7]);

        if (gray_sensor[0]) {
        }

        delay_us(1000);
    }
#else
#endif

    return SUCCESS;
}

void GrayScaleSensor_ReadAnalog(GrayScaleSensor_t *Self, uint8_t *Data) {
    if (Self->Mode == GrayScaleSensorMode_Analog) {
        GraySacleSensor_SWI2C_NowAddrReadBytes(Self, Self->Addr << 1, Data, 8);
    } else {
        GraySacleSensor_SWI2C_SingedAddrReadBytes(Self, Self->Addr << 1,
                                                  GW_GRAY_ANALOG_MODE, Data, 8);

        Self->Mode = GrayScaleSensorMode_Analog;
    }
}

ErrorStatus GrayScaleSensor_ScanAddress(GrayScaleSensor_t *Self) {
    uint8_t Temp;
    for (uint8_t i = 1; i < 127; i++) {
        if (GraySacleSensor_SWI2C_NowAddrReadBytes(Self, i << 1, &Temp, 1) ==
                0 &&
            Self->Addr == i) {

            return SUCCESS;
        }
    }

    return ERROR;
}
