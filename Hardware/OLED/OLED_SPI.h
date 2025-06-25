#ifndef OLED_SPI_H
#define OLED_SPI_H

#include "OLED.h"

typedef enum {
    SPI_Command,
    SPI_Data,
} OLED_SPI_ByteType;

void OLED_SWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length);
void OLED_SWSPI_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_SWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands, uint16_t Length);

void OLED_HWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length);
void OLED_HWSPI_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_HWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands, uint16_t Length);

#endif