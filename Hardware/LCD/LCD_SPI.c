#include "LCD_SPI.h"
#include "GPIO.h"

void LCD_SWSPI_Start(LCD_t *Self, uint8_t DC) {
    GPIO_Write(Self->DC_ODR, DC);
    GPIO_Write(Self->CS_ODR, 0);
}

void LCD_SWSPI_Stop(LCD_t *Self) {
    GPIO_Write(Self->CS_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
}

void LCD_SWSPI_WriteByte(LCD_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(Self->SCL_ODR, 0);
        GPIO_Write(Self->SDA_ODR, Byte & 0x80);
        GPIO_Write(Self->SCL_ODR, 1);
        Byte <<= 1;
    }
}

void LCD_SWSPI_Write2Byte(LCD_t *Self, uint16_t Byte) {
    LCD_SWSPI_WriteByte(Self, Byte >> 8);
    LCD_SWSPI_WriteByte(Self, Byte);
}

void LCD_SWSPI_WriteData8(LCD_t *Self, uint8_t Data) {
    LCD_SWSPI_Start(Self, LCD_Data);
    LCD_SWSPI_WriteByte(Self, Data);
    LCD_SWSPI_Stop(Self);
}

void LCD_SWSPI_WriteData16(LCD_t *Self, uint16_t Data) {
    LCD_SWSPI_Start(Self, LCD_Data);
    LCD_SWSPI_Write2Byte(Self, Data);
    LCD_SWSPI_Stop(Self);
}

void LCD_SWSPI_WriteData16s(LCD_t *Self, uint16_t Data, uint16_t Length) {
    LCD_SWSPI_Start(Self, LCD_Data);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_Write2Byte(Self, Data);
    }
    LCD_SWSPI_Stop(Self);
}

void LCD_SWSPI_WriteDatas16(LCD_t *Self, uint16_t *Datas, uint16_t Length) {
    LCD_SWSPI_Start(Self, LCD_Data);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_Write2Byte(Self, Datas[i]);
    }
    LCD_SWSPI_Stop(Self);
}

void LCD_SWSPI_WriteCommand(LCD_t *Self, uint8_t Command) {
    LCD_SWSPI_Start(Self, LCD_Command);
    LCD_SWSPI_WriteByte(Self, Command);
    LCD_SWSPI_Stop(Self);
}

void LCD_SWSPI_WriteCommands(LCD_t *Self, uint8_t *Commands, uint16_t Length) {
    LCD_SWSPI_Start(Self, LCD_Command);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_WriteByte(Self, Commands[i]);
    }
    LCD_SWSPI_Stop(Self);
}