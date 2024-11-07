#ifndef OLED_I2C_H
#define OLED_I2C_H

#include "OLED.h"

void OLED_WriteCommand(OLED_t *self, uint8_t Command);
void OLED_WriteData(OLED_t *self, uint8_t Data);

#endif