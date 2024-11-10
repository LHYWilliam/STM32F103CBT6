#include "OLED_I2C.h"
#include "GPIO.h"

#define OLED_WriteSCL(self, x) GPIO_Write(self->SCL_ODR, x)
#define OLED_WriteSDA(self, x) GPIO_Write(self->SDA_ODR, x)

void OLED_I2C_Start(OLED_t *self) {
    OLED_WriteSDA(self, 1);
    OLED_WriteSCL(self, 1);
    OLED_WriteSDA(self, 0);
    OLED_WriteSCL(self, 0);
}

void OLED_I2C_Stop(OLED_t *self) {
    OLED_WriteSDA(self, 0);
    OLED_WriteSCL(self, 1);
    OLED_WriteSDA(self, 1);
}

void OLED_I2C_SendByte(OLED_t *self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteSDA(self, Byte & (0x80 >> i));
        OLED_WriteSCL(self, 1);
        OLED_WriteSCL(self, 0);
    }
    OLED_WriteSCL(self, 1);
    OLED_WriteSCL(self, 0);
}

void OLED_SWI2C_WriteData(OLED_t *self, uint8_t Data) {
    OLED_I2C_Start(self);
    OLED_I2C_SendByte(self, 0x78);
    OLED_I2C_SendByte(self, 0x40);
    OLED_I2C_SendByte(self, Data);
    OLED_I2C_Stop(self);
}

void OLED_SWI2C_WriteCommand(OLED_t *self, uint8_t Command) {
    OLED_I2C_Start(self);
    OLED_I2C_SendByte(self, 0x78);
    OLED_I2C_SendByte(self, 0x00);
    OLED_I2C_SendByte(self, Command);
    OLED_I2C_Stop(self);
}

void OLED_HWI2C_WaitEvent(OLED_t *self, uint32_t I2C_EVENT, uint32_t Timeout) {
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT) != SUCCESS && Timeout--)
        ;
}

void OLED_HWI2C_WriteData(OLED_t *self, uint8_t Data) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_MODE_SELECT, 1000);

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,
                         1000);

    I2C_SendData(self->I2Cx, 0x40);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_BYTE_TRANSMITTING, 1000);

    I2C_SendData(self->I2Cx, Data);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_BYTE_TRANSMITTING, 1000);

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommand(OLED_t *self, uint8_t Command) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_MODE_SELECT, 1000);

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,
                         1000);

    I2C_SendData(self->I2Cx, 0x00);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_BYTE_TRANSMITTING, 1000);

    I2C_SendData(self->I2Cx, Command);
    OLED_HWI2C_WaitEvent(self, I2C_EVENT_MASTER_BYTE_TRANSMITTING, 1000);

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}
