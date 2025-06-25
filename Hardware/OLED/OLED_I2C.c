#include "OLED_I2C.h"
#include "GPIO.h"

#define OLED_WriteSCL(Self, x) GPIO_Write(Self->SCL_ODR, x)
#define OLED_WriteSDA(Self, x) GPIO_Write(Self->SDA_ODR, x)

void OLED_SWI2C_Start(OLED_t *Self) {
    OLED_WriteSDA(Self, 1);
    OLED_WriteSCL(Self, 1);
    OLED_WriteSDA(Self, 0);
    OLED_WriteSCL(Self, 0);
}

void OLED_SWI2C_Stop(OLED_t *Self) {
    OLED_WriteSDA(Self, 0);
    OLED_WriteSCL(Self, 1);
    OLED_WriteSDA(Self, 1);
}

void OLED_SWI2C_WriteByte(OLED_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteSDA(Self, Byte & (0x80 >> i));
        OLED_WriteSCL(Self, 1);
        OLED_WriteSCL(Self, 0);
    }
    OLED_WriteSCL(Self, 1);
    OLED_WriteSCL(Self, 0);
}

void OLED_SWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_SWI2C_WriteByte(Self, 0x40);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Data);
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_SWI2C_WriteByte(Self, 0x40);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Datas[i]);
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_SWI2C_WriteByte(Self, 0x00);
    OLED_SWI2C_WriteByte(Self, Command);
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_SWI2C_WriteByte(Self, 0x00);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Commands[i]);
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_HWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length) {
    I2C_GenerateSTART(Self->I2Cx, ENABLE);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(Self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(Self->I2Cx, 0x40);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(Self->I2Cx, Data);
        while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteDatas(OLED_t *Self, uint8_t *Data, uint16_t Length) {
    I2C_GenerateSTART(Self->I2Cx, ENABLE);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(Self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(Self->I2Cx, 0x40);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(Self->I2Cx, Data[i]);
        while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommand(OLED_t *Self, uint8_t Command) {
    I2C_GenerateSTART(Self->I2Cx, ENABLE);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(Self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(Self->I2Cx, 0x00);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    I2C_SendData(Self->I2Cx, Command);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommands(OLED_t *Self, uint8_t *Command, uint16_t Length) {
    I2C_GenerateSTART(Self->I2Cx, ENABLE);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    while (I2C_CheckEvent(Self->I2Cx,
                          I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
           SUCCESS)
        ;

    I2C_SendData(Self->I2Cx, 0x00);
    while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
           SUCCESS)
        ;

    for (uint16_t i = 0; i < Length; i++) {
        I2C_SendData(Self->I2Cx, Command[i]);
        while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
               SUCCESS)
            ;
    }

    I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}
