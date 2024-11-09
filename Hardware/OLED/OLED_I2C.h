#ifndef OLED_I2C_H
#define OLED_I2C_H

#include "OLED.h"

void OLED_I2C_WriteData(OLED_t *self, uint8_t Data);
void OLED_I2C_WriteCommand(OLED_t *self, uint8_t Command);

#endif