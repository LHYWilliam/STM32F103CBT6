#include "Delay.h"
#include "GPIO.h"

#include "GrayScaleSensor_I2C.h"

void GraySacleSensor_SWI2C_Init(GrayScaleSensor_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_Mode_Out_OD;
    GPIO_InitPin(&GPIO, Self->SCL);
    GPIO_InitPin(&GPIO, Self->SDA);

    Self->SCL_GPIO = GPIOx(Self->SCL);
    Self->SCL_Pin  = GPIO_Pinx(Self->SCL);
    Self->SDA_GPIO = GPIOx(Self->SDA);
    Self->SDA_Pin  = GPIO_Pinx(Self->SDA);

    GPIO_SetBits(Self->SCL_GPIO, Self->SCL_Pin);
    GPIO_SetBits(Self->SDA_GPIO, Self->SDA_Pin);
}

void GraySacleSensor_SWI2C_SDAOut(GrayScaleSensor_t *Self, uint8_t Bit) {
    GPIO_WriteBit(Self->SDA_GPIO, Self->SDA_Pin, (BitAction)Bit);

    Delay_us(10);
}

uint8_t GraySacleSensor_SWI2C_SDAIn(GrayScaleSensor_t *Self) {
    uint8_t bit;
    bit = (uint8_t)GPIO_ReadInputDataBit(Self->SDA_GPIO, Self->SDA_Pin);

    Delay_us(10);
    return bit;
}

void GraySacleSensor_SWI2C_SCLOut(GrayScaleSensor_t *Self, uint8_t Bit) {
    GPIO_WriteBit(Self->SCL_GPIO, Self->SCL_Pin, (BitAction)Bit);

    Delay_us(10);
}

void GraySacleSensor_SWI2C_Start(GrayScaleSensor_t *Self) {
    GraySacleSensor_SWI2C_SDAOut(Self, HIGH);
    GraySacleSensor_SWI2C_SCLOut(Self, HIGH);
    GraySacleSensor_SWI2C_SDAOut(Self, LOW);
    GraySacleSensor_SWI2C_SCLOut(Self, LOW);
}

void GraySacleSensor_SWI2C_Stop(GrayScaleSensor_t *Self) {
    GraySacleSensor_SWI2C_SDAOut(Self, LOW);
    GraySacleSensor_SWI2C_SCLOut(Self, HIGH);
    GraySacleSensor_SWI2C_SDAOut(Self, HIGH);
}

void GraySacleSensor_SWI2C_WriteBit(GrayScaleSensor_t *Self, uint8_t Bit) {
    GraySacleSensor_SWI2C_SDAOut(Self, Bit);
    GraySacleSensor_SWI2C_SCLOut(Self, HIGH);
    GraySacleSensor_SWI2C_SCLOut(Self, LOW);
}

uint8_t GraySacleSensor_SWI2C_ReadBit(GrayScaleSensor_t *Self) {
    GraySacleSensor_SWI2C_SDAOut(Self, HIGH);
    GraySacleSensor_SWI2C_SCLOut(Self, HIGH);
    uint8_t Bit = GraySacleSensor_SWI2C_SDAIn(Self);
    GraySacleSensor_SWI2C_SCLOut(Self, LOW);

    return Bit;
}

uint8_t GraySacleSensor_SWI2C_WriteByte(GrayScaleSensor_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; ++i) {
        GraySacleSensor_SWI2C_WriteBit(Self, Byte & (0x80 >> i));
    }

    uint8_t Ack = GraySacleSensor_SWI2C_ReadBit(Self);

    return Ack;
}

uint8_t GraySacleSensor_SWI2C_ReadByte(GrayScaleSensor_t *Self, uint8_t Ack) {
    uint8_t Byte = 0;

    GraySacleSensor_SWI2C_SDAOut(Self, HIGH);

    for (uint8_t i = 0; i < 8; ++i) {
        GraySacleSensor_SWI2C_SCLOut(Self, HIGH);
        Byte <<= 1;
        Byte |= GraySacleSensor_SWI2C_SDAIn(Self);
        GraySacleSensor_SWI2C_SCLOut(Self, LOW);
    }

    GraySacleSensor_SWI2C_WriteBit(Self, Ack);

    return Byte;
}

int8_t GraySacleSensor_SWI2C_NowAddrReadBytes(GrayScaleSensor_t *Self,
                                              uint8_t DevAddr, uint8_t *Bytes,
                                              uint8_t Length) {

    GraySacleSensor_SWI2C_Start(Self);

    uint8_t Ack = GraySacleSensor_SWI2C_WriteByte(Self, DevAddr | I2C_READ);
    if (Ack) {
        GraySacleSensor_SWI2C_Stop(Self);
        return 1;
    }

    uint8_t i;
    for (i = 0; i < Length - 1; ++i) {
        Bytes[i] = GraySacleSensor_SWI2C_ReadByte(Self, ACK);
    }

    Bytes[i] = GraySacleSensor_SWI2C_ReadByte(Self, NACK);

    GraySacleSensor_SWI2C_Stop(Self);

    return 0;
}

int8_t GraySacleSensor_SWI2C_NowAddrWriteBytes(GrayScaleSensor_t *Self,
                                               uint8_t            DevAddr,
                                               const uint8_t     *Bytes,
                                               uint8_t            Length) {
    GraySacleSensor_SWI2C_Start(Self);

    uint8_t Ack = GraySacleSensor_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GraySacleSensor_SWI2C_Stop(Self);
        return 1;
    }

    for (uint8_t i = 0; i < Length; ++i) {
        Ack = GraySacleSensor_SWI2C_WriteByte(Self, Bytes[i]);
    }

    GraySacleSensor_SWI2C_Stop(Self);

    return 0;
}

int8_t GraySacleSensor_SWI2C_NowAddrReadByte(GrayScaleSensor_t *Self,
                                             uint8_t DevAddr, uint8_t *data) {
    return GraySacleSensor_SWI2C_NowAddrReadBytes(Self, DevAddr, data, 1);
}

int8_t GraySacleSensor_SWI2C_NowAddrWriteByte(GrayScaleSensor_t *Self,
                                              uint8_t            DevAddr,
                                              const uint8_t      Byte) {
    return GraySacleSensor_SWI2C_NowAddrWriteBytes(Self, DevAddr, &Byte, 1);
}

int8_t GraySacleSensor_SWI2C_SingedAddrReadBytes(GrayScaleSensor_t *Self,
                                                 uint8_t            DevAddr,
                                                 uint8_t            MemAddr,
                                                 uint8_t           *Bytes,
                                                 uint8_t            Length) {
    GraySacleSensor_SWI2C_Start(Self);

    uint8_t Ack = GraySacleSensor_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GraySacleSensor_SWI2C_Stop(Self);
        return 1;
    }
    Ack = GraySacleSensor_SWI2C_WriteByte(Self, MemAddr);

    return GraySacleSensor_SWI2C_NowAddrReadBytes(Self, DevAddr, Bytes, Length);
}

int8_t GraySacleSensor_SWI2C_SingedAddrWriteBytes(GrayScaleSensor_t *Self,
                                                  uint8_t            DevAddr,
                                                  uint8_t            MemAddr,
                                                  const uint8_t     *Bytes,
                                                  uint8_t            Length) {
    GraySacleSensor_SWI2C_Start(Self);

    uint8_t Ack = GraySacleSensor_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GraySacleSensor_SWI2C_Stop(Self);
        return 1;
    }
    Ack = GraySacleSensor_SWI2C_WriteByte(Self, MemAddr);

    return GraySacleSensor_SWI2C_NowAddrWriteBytes(Self, DevAddr, Bytes,
                                                   Length);
}
