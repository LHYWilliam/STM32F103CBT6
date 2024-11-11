#ifndef OLED_SPI_H
#define OLED_SPI_H

#include "OLED.h"

typedef enum {
    SPI_Command,
    SPI_Data,
} OLED_SPI_Type_t;

void OLED_SWSPI_WriteData(OLED_t *self, uint8_t Data, uint16_t Length);
void OLED_SWSPI_WriteDatas(OLED_t *self, uint8_t *Datas, uint16_t Length);
void OLED_SWSPI_WriteCommand(OLED_t *self, uint8_t Command);
void OLED_SWSPI_WriteCommands(OLED_t *self, uint8_t *Commands, uint16_t Length);

#endif