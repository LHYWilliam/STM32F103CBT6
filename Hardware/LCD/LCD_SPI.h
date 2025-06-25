#ifndef LCD_SPI_H
#define LCD_SPI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "LCD.h"

typedef enum {
    LCD_Command,
    LCD_Data,
} LCD_ByteType;

void LCD_SWSPI_WriteData8(LCD_t *Self, uint8_t Data);
void LCD_SWSPI_WriteData16(LCD_t *Self, uint16_t Data);
void LCD_SWSPI_WriteData16s(LCD_t *Self, uint16_t Data, uint16_t Length);
void LCD_SWSPI_WriteDatas16(LCD_t *Self, uint16_t *Datas, uint16_t Length);

void LCD_SWSPI_WriteCommand(LCD_t *Self, uint8_t Command);
void LCD_SWSPI_WriteCommands(LCD_t *Self, uint8_t *Commands, uint16_t Length);

#endif