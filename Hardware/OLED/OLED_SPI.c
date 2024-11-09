#include "OLED_SPI.h"
#include "GPIO.h"
#include "OLED.h"

void OLED_SPI_WriteByte(OLED_t *self, uint8_t Byte, uint8_t DC) {
    GPIO_Write(self->DC_ODR, DC);
    GPIO_Write(self->CS_ODR, 0);

    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(self->D0_ODR, 0);
        GPIO_Write(self->D1_ODR, Byte & 0x80);
        GPIO_Write(self->D0_ODR, 1);
        Byte <<= 1;
    }

    GPIO_Write(self->CS_ODR, 1);
    GPIO_Write(self->DC_ODR, 1);
}

void OLED_SPI_WriteData(OLED_t *self, uint8_t Data) {
    OLED_SPI_WriteByte(self, Data, SPI_Data);
}

void OLED_SPI_WriteCommand(OLED_t *self, uint8_t Command) {
    OLED_SPI_WriteByte(self, Command, SPI_Command);
}