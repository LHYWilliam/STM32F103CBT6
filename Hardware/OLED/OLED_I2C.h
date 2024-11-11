#ifndef OLED_I2C_H
#define OLED_I2C_H

#include "OLED.h"

void OLED_SWI2C_WriteData(OLED_t *self, uint8_t Data, uint16_t Length);
void OLED_SWI2C_WriteDatas(OLED_t *self, uint8_t *Datas, uint16_t Length);
void OLED_SWI2C_WriteCommand(OLED_t *self, uint8_t Command);
void OLED_SWI2C_WriteCommands(OLED_t *self, uint8_t *Commands, uint16_t Length);

void OLED_HWI2C_WriteData(OLED_t *self, uint8_t Data, uint16_t Length);
void OLED_HWI2C_WriteDatas(OLED_t *self, uint8_t *Data, uint16_t Length);
void OLED_HWI2C_WriteCommand(OLED_t *self, uint8_t Command);
void OLED_HWI2C_WriteCommands(OLED_t *self, uint8_t *Command, uint16_t Length);

#endif