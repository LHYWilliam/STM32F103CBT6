#ifndef LCD_SPI_H
#define LCD_SPI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "LCD.h"

typedef enum {
    LCD_Command,
    LCD_Data,
} LCD_ByteType;

void LCD_SWSPI_WriteByte(LCD_t *self, uint8_t Byte);
void LCD_SWSPI_Write2Byte(LCD_t *self, uint16_t Byte);

void LCD_SWSPI_WriteData(LCD_t *self, uint16_t Data, uint16_t Length);
void LCD_SWSPI_WriteDatas(LCD_t *self, uint16_t *Datas, uint16_t Length);

void LCD_SWSPI_WriteCommand(LCD_t *self, uint8_t Command);
void LCD_SWSPI_WriteCommands(LCD_t *self, uint8_t *Commands, uint16_t Length);

#endif