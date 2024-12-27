#include "LCD.h"
#include "Delay.h"
#include "LCD_SPI.h"

void LCD_SetWindow(LCD_t *self, uint16_t x1, uint16_t y1, uint16_t x2,
                   uint16_t y2) {
    if (self->Direction == 0) {
        self->WriteCommand(self, 0x2a);
        self->Write2Byte(self, x1 + 2);
        self->Write2Byte(self, x2 + 2);
        self->WriteCommand(self, 0x2b);
        self->Write2Byte(self, y1 + 1);
        self->Write2Byte(self, y2 + 1);
        self->WriteCommand(self, 0x2c);

    } else if (self->Direction == 1) {
        self->WriteCommand(self, 0x2a);
        self->Write2Byte(self, x1 + 2);
        self->Write2Byte(self, x2 + 2);
        self->WriteCommand(self, 0x2b);
        self->Write2Byte(self, y1 + 1);
        self->Write2Byte(self, y2 + 1);
        self->WriteCommand(self, 0x2c);

    } else if (self->Direction == 2) {
        self->WriteCommand(self, 0x2a);
        self->Write2Byte(self, x1 + 1);
        self->Write2Byte(self, x2 + 1);
        self->WriteCommand(self, 0x2b);
        self->Write2Byte(self, y1 + 2);
        self->Write2Byte(self, y2 + 2);
        self->WriteCommand(self, 0x2c);

    } else if (self->Direction == 3) {
        self->WriteCommand(self, 0x2a);
        self->Write2Byte(self, x1 + 1);
        self->Write2Byte(self, x2 + 1);
        self->WriteCommand(self, 0x2b);
        self->Write2Byte(self, y1 + 2);
        self->Write2Byte(self, y2 + 2);
        self->WriteCommand(self, 0x2c);
    }
}

void LCD_Fill(LCD_t *self, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2,
              uint16_t Color) {
    LCD_SetWindow(self, X1, Y1, X2, Y2);

    for (uint16_t i = Y1; i < Y2 + 1; i++) {
        for (uint16_t j = X1; j < X2 + 1; j++) {
            self->Write2Byte(self, Color);
        }
    }
}

void LCD_Clear(LCD_t *self) {
    LCD_Fill(self, 0, 0, self->Width - 1, self->Height - 1, self->BackColor);
}

void LCD_SetPenColor(LCD_t *self, uint16_t Color) { self->PenColor = Color; }
void LCD_SetBackColor(LCD_t *self, uint16_t Color) { self->BackColor = Color; }

void LCD_Init(LCD_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;

    self->SCL_ODR = GPIO_InitPin(&GPIO, self->SCL);
    self->SDA_ODR = GPIO_InitPin(&GPIO, self->SDA);
    self->RES_ODR = GPIO_InitPin(&GPIO, self->RES);
    self->DC_ODR = GPIO_InitPin(&GPIO, self->DC);
    self->CS_ODR = GPIO_InitPin(&GPIO, self->CS);
    self->BL_ODR = GPIO_InitPin(&GPIO, self->BL);

    GPIO_Write(self->SCL_ODR, 1);
    GPIO_Write(self->SDA_ODR, 1);
    GPIO_Write(self->RES_ODR, 1);
    GPIO_Write(self->DC_ODR, 1);
    GPIO_Write(self->CS_ODR, 1);
    GPIO_Write(self->BL_ODR, 0);

    self->WriteByte = LCD_SWSPI_WriteByte;
    self->Write2Byte = LCD_SWSPI_Write2Byte;
    self->WriteCommand = LCD_SWSPI_WriteCommand;

    GPIO_Write(self->RES_ODR, 0);
    Delay_ms(10);
    GPIO_Write(self->RES_ODR, 1);
    Delay_ms(10);

    self->WriteCommand(self, 0x11);

    self->WriteCommand(self, 0xB1);
    self->WriteByte(self, 0x05);
    self->WriteByte(self, 0x3C);
    self->WriteByte(self, 0x3C);
    self->WriteCommand(self, 0xB2);
    self->WriteByte(self, 0x05);
    self->WriteByte(self, 0x3C);
    self->WriteByte(self, 0x3C);
    self->WriteCommand(self, 0xB3);
    self->WriteByte(self, 0x05);
    self->WriteByte(self, 0x3C);
    self->WriteByte(self, 0x3C);
    self->WriteByte(self, 0x05);
    self->WriteByte(self, 0x3C);
    self->WriteByte(self, 0x3C);

    self->WriteCommand(self, 0xB4);
    self->WriteByte(self, 0x03);

    self->WriteCommand(self, 0xC0);
    self->WriteByte(self, 0x28);
    self->WriteByte(self, 0x08);
    self->WriteByte(self, 0x04);
    self->WriteCommand(self, 0xC1);
    self->WriteByte(self, 0XC0);
    self->WriteCommand(self, 0xC2);
    self->WriteByte(self, 0x0D);
    self->WriteByte(self, 0x00);
    self->WriteCommand(self, 0xC3);
    self->WriteByte(self, 0x8D);
    self->WriteByte(self, 0x2A);
    self->WriteCommand(self, 0xC4);
    self->WriteByte(self, 0x8D);
    self->WriteByte(self, 0xEE);

    self->WriteCommand(self, 0xC5);
    self->WriteByte(self, 0x1A);
    self->WriteCommand(self, 0x36);
    if (self->Direction == 0) {
        self->WriteByte(self, 0x00);

    } else if (self->Direction == 1) {
        self->WriteByte(self, 0xC0);

    } else if (self->Direction == 2) {
        self->WriteByte(self, 0x70);

    } else if (self->Direction == 3) {
        self->WriteByte(self, 0xA0);
    }

    self->WriteCommand(self, 0xE0);
    self->WriteByte(self, 0x04);
    self->WriteByte(self, 0x22);
    self->WriteByte(self, 0x07);
    self->WriteByte(self, 0x0A);
    self->WriteByte(self, 0x2E);
    self->WriteByte(self, 0x30);
    self->WriteByte(self, 0x25);
    self->WriteByte(self, 0x2A);
    self->WriteByte(self, 0x28);
    self->WriteByte(self, 0x26);
    self->WriteByte(self, 0x2E);
    self->WriteByte(self, 0x3A);
    self->WriteByte(self, 0x00);
    self->WriteByte(self, 0x01);
    self->WriteByte(self, 0x03);
    self->WriteByte(self, 0x13);
    self->WriteCommand(self, 0xE1);
    self->WriteByte(self, 0x04);
    self->WriteByte(self, 0x16);
    self->WriteByte(self, 0x06);
    self->WriteByte(self, 0x0D);
    self->WriteByte(self, 0x2D);
    self->WriteByte(self, 0x26);
    self->WriteByte(self, 0x23);
    self->WriteByte(self, 0x27);
    self->WriteByte(self, 0x27);
    self->WriteByte(self, 0x25);
    self->WriteByte(self, 0x2D);
    self->WriteByte(self, 0x3B);
    self->WriteByte(self, 0x00);
    self->WriteByte(self, 0x01);
    self->WriteByte(self, 0x04);
    self->WriteByte(self, 0x13);

    self->WriteCommand(self, 0x3A);
    self->WriteByte(self, 0x05);
    self->WriteCommand(self, 0x29);

    LCD_Clear(self);

    GPIO_Write(self->BL_ODR, 1);
}