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

void OLED_I2C_WriteData(OLED_t *self, uint8_t Data) {
    OLED_I2C_Start(self);
    OLED_I2C_SendByte(self, 0x78);
    OLED_I2C_SendByte(self, 0x40);
    OLED_I2C_SendByte(self, Data);
    OLED_I2C_Stop(self);
}

void OLED_I2C_WriteCommand(OLED_t *self, uint8_t Command) {
    OLED_I2C_Start(self);
    OLED_I2C_SendByte(self, 0x78);
    OLED_I2C_SendByte(self, 0x00);
    OLED_I2C_SendByte(self, Command);
    OLED_I2C_Stop(self);
}