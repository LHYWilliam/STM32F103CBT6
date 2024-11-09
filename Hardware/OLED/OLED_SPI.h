#ifndef OLED_SPI_H
#define OLED_SPI_H

#include "OLED.h"

typedef enum {
    SPI_Command,
    SPI_Data,
} OLED_SPI_Type_t;

void OLED_SPI_WriteData(OLED_t *self, uint8_t Data);
void OLED_SPI_WriteCommand(OLED_t *self, uint8_t Command);

#endif