#include "LCD_SPI.h"
#include "GPIO.h"

void LCD_SWSPI_Start(LCD_t *self, uint8_t DC) {
    GPIO_Write(self->DC_ODR, DC);
    GPIO_Write(self->CS_ODR, 0);
}

void LCD_SWSPI_Stop(LCD_t *self) {
    GPIO_Write(self->CS_ODR, 1);
    GPIO_Write(self->DC_ODR, 1);
}

void LCD_SWSPI_WriteByte(LCD_t *self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(self->SCL_ODR, 0);
        GPIO_Write(self->SDA_ODR, Byte & 0x80);
        GPIO_Write(self->SCL_ODR, 1);
        Byte <<= 1;
    }
}

void LCD_SWSPI_Write2Byte(LCD_t *self, uint16_t Byte) {
    LCD_SWSPI_WriteByte(self, Byte >> 8);
    LCD_SWSPI_WriteByte(self, Byte);
}

void LCD_SWSPI_WriteData(LCD_t *self, uint16_t Data, uint16_t Length) {
    LCD_SWSPI_Start(self, LCD_Data);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_Write2Byte(self, Data);
    }
    LCD_SWSPI_Stop(self);
}

void LCD_SWSPI_WriteDatas(LCD_t *self, uint16_t *Datas, uint16_t Length) {
    LCD_SWSPI_Start(self, LCD_Data);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_Write2Byte(self, Datas[i]);
    }
    LCD_SWSPI_Stop(self);
}

void LCD_SWSPI_WriteCommand(LCD_t *self, uint8_t Command) {
    LCD_SWSPI_Start(self, LCD_Command);
    LCD_SWSPI_WriteByte(self, Command);
    LCD_SWSPI_Stop(self);
}

void LCD_SWSPI_WriteCommands(LCD_t *self, uint8_t *Commands, uint16_t Length) {
    LCD_SWSPI_Start(self, LCD_Command);
    for (uint16_t i = 0; i < Length; i++) {
        LCD_SWSPI_WriteByte(self, Commands[i]);
    }
    LCD_SWSPI_Stop(self);
}