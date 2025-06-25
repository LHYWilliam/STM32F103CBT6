#include "OLED_SPI.h"
#include "GPIO.h"

#include "Delay.h"

void OLED_SWSPI_Start(OLED_t *Self, uint8_t DC) {
    GPIO_Write(Self->DC_ODR, DC);
    GPIO_Write(Self->CS_ODR, 0);
}

void OLED_SWSPI_Stop(OLED_t *Self) {
    GPIO_Write(Self->CS_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
}

void OLED_SWSPI_WriteByte(OLED_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(Self->D0_ODR, 0);
        GPIO_Write(Self->D1_ODR, Byte & 0x80);
        GPIO_Write(Self->D0_ODR, 1);
        Byte <<= 1;
    }
}

void OLED_SWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(Self, Datas[i]);
    }
    OLED_SWSPI_Stop(Self);
}

void OLED_SWSPI_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWSPI_Start(Self, SPI_Command);
    OLED_SWSPI_WriteByte(Self, Command);
    OLED_SWSPI_Stop(Self);
}

void OLED_SWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Command);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(Self, Commands[i]);
    }
    OLED_SWSPI_Stop(Self);
}

#define OLED_HWSPI_Delay() Delay_us(1)

void OLED_HWSPI_WriteByte(OLED_t *Self, uint8_t Byte) {
    while (SPI_I2S_GetFlagStatus(Self->SPIx, SPI_I2S_FLAG_TXE) != SET)
        ;
    SPI_I2S_SendData(Self->SPIx, Byte);
}

void OLED_HWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_HWSPI_WriteByte(Self, Datas[i]);
        OLED_HWSPI_Delay();
    }
    OLED_SWSPI_Stop(Self);
}

void OLED_HWSPI_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWSPI_Start(Self, SPI_Command);
    OLED_HWSPI_WriteByte(Self, Command);
    OLED_HWSPI_Delay();
    OLED_SWSPI_Stop(Self);
}

void OLED_HWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Command);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_HWSPI_WriteByte(Self, Commands[i]);
    }
    OLED_SWSPI_Stop(Self);
}