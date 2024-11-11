#include "OLED_I2C.h"
#include "GPIO.h"

#define OLED_WriteSCL(self, x) GPIO_Write(self->SCL_ODR, x)
#define OLED_WriteSDA(self, x) GPIO_Write(self->SDA_ODR, x)

void OLED_SWI2C_Start(OLED_t *self) {
    OLED_WriteSDA(self, 1);
    OLED_WriteSCL(self, 1);
    OLED_WriteSDA(self, 0);
    OLED_WriteSCL(self, 0);
}

void OLED_SWI2C_Stop(OLED_t *self) {
    OLED_WriteSDA(self, 0);
    OLED_WriteSCL(self, 1);
    OLED_WriteSDA(self, 1);
}

void OLED_SWI2C_WriteByte(OLED_t *self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteSDA(self, Byte & (0x80 >> i));
        OLED_WriteSCL(self, 1);
        OLED_WriteSCL(self, 0);
    }
    OLED_WriteSCL(self, 1);
    OLED_WriteSCL(self, 0);
}

void OLED_SWI2C_WriteData(OLED_t *self, uint8_t Data, uint16_t Length) {
    OLED_SWI2C_Start(self);
    OLED_SWI2C_WriteByte(self, 0x78);
    OLED_SWI2C_WriteByte(self, 0x40);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(self, Data);
    }
    OLED_SWI2C_Stop(self);
}

void OLED_SWI2C_WriteDatas(OLED_t *self, uint8_t *Datas, uint16_t Length) {
    OLED_SWI2C_Start(self);
    OLED_SWI2C_WriteByte(self, 0x78);
    OLED_SWI2C_WriteByte(self, 0x40);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(self, Datas[i]);
    }
    OLED_SWI2C_Stop(self);
}

void OLED_SWI2C_WriteCommand(OLED_t *self, uint8_t Command) {
    OLED_SWI2C_Start(self);
    OLED_SWI2C_WriteByte(self, 0x78);
    OLED_SWI2C_WriteByte(self, 0x00);
    OLED_SWI2C_WriteByte(self, Command);
    OLED_SWI2C_Stop(self);
}

void OLED_SWI2C_WriteCommands(OLED_t *self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWI2C_Start(self);
    OLED_SWI2C_WriteByte(self, 0x78);
    OLED_SWI2C_WriteByte(self, 0x00);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(self, Commands[i]);
    }
    OLED_SWI2C_Stop(self);
}

void OLED_HWI2C_WriteData(OLED_t *self, uint8_t Data, uint16_t Length) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(self->I2Cx, 0x40);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(self->I2Cx, Data);
        while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteDatas(OLED_t *self, uint8_t *Data, uint16_t Length) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(self->I2Cx, 0x40);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(self->I2Cx, Data[i]);
        while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommand(OLED_t *self, uint8_t Command) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(self->I2Cx, 0x00);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    I2C_SendData(self->I2Cx, Command);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommands(OLED_t *self, uint8_t *Command, uint16_t Length) {
    I2C_GenerateSTART(self->I2Cx, ENABLE);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(self->I2Cx, 0x00);
    while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(self->I2Cx, Command[i]);
        while (I2C_CheckEvent(self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(self->I2Cx, ENABLE);
}
