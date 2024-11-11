#include "OLED_SPI.h"
#include "GPIO.h"

void OLED_SWSPI_Start(OLED_t *self, uint8_t DC) {
    GPIO_Write(self->DC_ODR, DC);
    GPIO_Write(self->CS_ODR, 0);
}

void OLED_SWSPI_Stop(OLED_t *self) {
    GPIO_Write(self->CS_ODR, 1);
    GPIO_Write(self->DC_ODR, 1);
}

void OLED_SWSPI_WriteByte(OLED_t *self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(self->D0_ODR, 0);
        GPIO_Write(self->D1_ODR, Byte & 0x80);
        GPIO_Write(self->D0_ODR, 1);
        Byte <<= 1;
    }
}

void OLED_SWSPI_WriteData(OLED_t *self, uint8_t Data, uint16_t Length) {
    OLED_SWSPI_Start(self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(self, Data);
    }
    OLED_SWSPI_Stop(self);
}

void OLED_SWSPI_WriteDatas(OLED_t *self, uint8_t *Datas, uint16_t Length) {
    OLED_SWSPI_Start(self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(self, Datas[i]);
    }
    OLED_SWSPI_Stop(self);
}

void OLED_SWSPI_WriteCommand(OLED_t *self, uint8_t Command) {
    OLED_SWSPI_Start(self, SPI_Command);
    OLED_SWSPI_WriteByte(self, Command);
    OLED_SWSPI_Stop(self);
}

void OLED_SWSPI_WriteCommands(OLED_t *self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWSPI_Start(self, SPI_Command);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(self, Commands[i]);
    }
    OLED_SWSPI_Stop(self);
}