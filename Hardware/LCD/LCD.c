#include <stdarg.h>

#include "Delay.h"
#include "LCD.h"
#include "LCD_SPI.h"

void LCD_SetWindow(LCD_t *self, uint16_t X1, uint16_t Y1, uint16_t Width,
                   uint16_t Height);
void LCD_SetCusor(LCD_t *self, uint16_t X, uint16_t Y);

void LCD_SetPenColor(LCD_t *self, uint16_t Color) { self->PenColor = Color; }

void LCD_SetBackColor(LCD_t *self, uint16_t Color) { self->BackColor = Color; }

void LCD_SetFont(LCD_t *self, uint32_t Font) {
    self->Font = Font;
    for (uint8_t i = 0; i < sizeof(LCDFonts) / sizeof(LCDFonts[0]); i++) {
        if (LCDFonts[i].Font == self->Font) {
            self->FontWidth  = LCDFonts[i].FontWidth;
            self->FontHeight = LCDFonts[i].FontHeight;
            break;
        }
    }
}

void LCD_Fill(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Width,
              uint16_t Height, uint16_t Color) {
    LCD_SetWindow(self, X, Y, Width, Height);
    self->WriteData16s(self, Color, Width * Height);
}

void LCD_Clear(LCD_t *self) {
    LCD_Fill(self, 0, 0, self->Width, self->Height, self->BackColor);
}

void LCD_DrawPoint(LCD_t *self, uint16_t X, uint16_t Y) {
    LCD_SetCusor(self, X, Y);
    self->WriteData16(self, self->PenColor);
}

void LCD_DrawHLine(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Length) {
    LCD_SetWindow(self, X, Y, Length, 1);
    self->WriteData16s(self, self->PenColor, Length);
}

void LCD_DrawVLine(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Length) {
    LCD_SetWindow(self, X, Y, 1, Length);
    self->WriteData16s(self, self->PenColor, Length);
}

void LCD_ShowChar(LCD_t *self, uint16_t X, uint16_t Y, uint8_t Char) {
    LCD_SetWindow(self, X, Y, self->FontWidth, self->FontHeight);

    uint8_t bytePerLine = (self->FontWidth + 7) / 8;
    for (uint16_t Line = 0; Line < self->FontHeight; Line++) {
        for (uint16_t Column = 0; Column < self->FontWidth; Column++) {
            if (self->Font == (uint32_t)LCD_Font8x16) {
                if (LCD_Font8x16[Char - ' '][Line * bytePerLine + Column / 8] &
                    (0b10000000 >> (Column % 8))) {
                    self->WriteData16(self, self->PenColor);

                } else {
                    self->WriteData16(self, self->BackColor);
                }
            }
        }
    }
}

void LCD_ShowString(LCD_t *self, uint16_t X, uint16_t Y, char *String) {
    for (uint8_t i = 0; String[i]; i++) {
        LCD_ShowChar(self, X, Y, String[i]);
        X += self->FontWidth;
    }
}

void LCD_Printf(LCD_t *self, int16_t X, int16_t Y, const char *Format, ...) {
    va_list arg;
    va_start(arg, Format);
    vsprintf((char *)self->PrintfBuffer, Format, arg);
    va_end(arg);

    LCD_ShowString(self, X, Y, (char *)self->PrintfBuffer);
}

void LCD_ShowImage(LCD_t *self, uint16_t X, uint16_t Y, uint16_t Width,
                   uint16_t Height, const uint8_t *Image) {
    LCD_SetWindow(self, X, Y, Width, Height);

    self->WriteDatas16(self, (uint16_t *)Image, Width * Height);
}

void LCD_Init(LCD_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;

    self->SCL_ODR = GPIO_InitPin(&GPIO, self->SCL);
    self->SDA_ODR = GPIO_InitPin(&GPIO, self->SDA);
    self->RES_ODR = GPIO_InitPin(&GPIO, self->RES);
    self->DC_ODR  = GPIO_InitPin(&GPIO, self->DC);
    self->CS_ODR  = GPIO_InitPin(&GPIO, self->CS);
    self->BL_ODR  = GPIO_InitPin(&GPIO, self->BL);

    GPIO_Write(self->SCL_ODR, 1);
    GPIO_Write(self->SDA_ODR, 1);
    GPIO_Write(self->RES_ODR, 1);
    GPIO_Write(self->DC_ODR, 1);
    GPIO_Write(self->CS_ODR, 1);
    GPIO_Write(self->BL_ODR, 0);

    self->WriteData8   = LCD_SWSPI_WriteData8;
    self->WriteData16  = LCD_SWSPI_WriteData16;
    self->WriteData16s = LCD_SWSPI_WriteData16s;
    self->WriteDatas16 = LCD_SWSPI_WriteDatas16;
    self->WriteCommand = LCD_SWSPI_WriteCommand;

    GPIO_Write(self->RES_ODR, 0);
    Delay_ms(100);
    GPIO_Write(self->RES_ODR, 1);
    Delay_ms(100);

    GPIO_Write(self->BL_ODR, 1);
    Delay_ms(100);

    self->WriteCommand(self, 0x11);
    Delay_ms(120);

    self->WriteCommand(self, 0xB1);
    self->WriteData8(self, 0x05);
    self->WriteData8(self, 0x3C);
    self->WriteData8(self, 0x3C);
    self->WriteCommand(self, 0xB2);
    self->WriteData8(self, 0x05);
    self->WriteData8(self, 0x3C);
    self->WriteData8(self, 0x3C);
    self->WriteCommand(self, 0xB3);
    self->WriteData8(self, 0x05);
    self->WriteData8(self, 0x3C);
    self->WriteData8(self, 0x3C);
    self->WriteData8(self, 0x05);
    self->WriteData8(self, 0x3C);
    self->WriteData8(self, 0x3C);

    self->WriteCommand(self, 0xB4);
    self->WriteData8(self, 0x03);

    self->WriteCommand(self, 0xC0);
    self->WriteData8(self, 0x28);
    self->WriteData8(self, 0x08);
    self->WriteData8(self, 0x04);
    self->WriteCommand(self, 0xC1);
    self->WriteData8(self, 0XC0);
    self->WriteCommand(self, 0xC2);
    self->WriteData8(self, 0x0D);
    self->WriteData8(self, 0x00);
    self->WriteCommand(self, 0xC3);
    self->WriteData8(self, 0x8D);
    self->WriteData8(self, 0x2A);
    self->WriteCommand(self, 0xC4);
    self->WriteData8(self, 0x8D);
    self->WriteData8(self, 0xEE);

    self->WriteCommand(self, 0xC5);
    self->WriteData8(self, 0x1A);
    self->WriteCommand(self, 0x36);
    if (self->Direction == 0) {
        self->WriteData8(self, 0x00);

    } else if (self->Direction == 1) {
        self->WriteData8(self, 0xC0);

    } else if (self->Direction == 2) {
        self->WriteData8(self, 0x70);

    } else if (self->Direction == 3) {
        self->WriteData8(self, 0xA0);
    }

    self->WriteCommand(self, 0xE0);
    self->WriteData8(self, 0x04);
    self->WriteData8(self, 0x22);
    self->WriteData8(self, 0x07);
    self->WriteData8(self, 0x0A);
    self->WriteData8(self, 0x2E);
    self->WriteData8(self, 0x30);
    self->WriteData8(self, 0x25);
    self->WriteData8(self, 0x2A);
    self->WriteData8(self, 0x28);
    self->WriteData8(self, 0x26);
    self->WriteData8(self, 0x2E);
    self->WriteData8(self, 0x3A);
    self->WriteData8(self, 0x00);
    self->WriteData8(self, 0x01);
    self->WriteData8(self, 0x03);
    self->WriteData8(self, 0x13);
    self->WriteCommand(self, 0xE1);
    self->WriteData8(self, 0x04);
    self->WriteData8(self, 0x16);
    self->WriteData8(self, 0x06);
    self->WriteData8(self, 0x0D);
    self->WriteData8(self, 0x2D);
    self->WriteData8(self, 0x26);
    self->WriteData8(self, 0x23);
    self->WriteData8(self, 0x27);
    self->WriteData8(self, 0x27);
    self->WriteData8(self, 0x25);
    self->WriteData8(self, 0x2D);
    self->WriteData8(self, 0x3B);
    self->WriteData8(self, 0x00);
    self->WriteData8(self, 0x01);
    self->WriteData8(self, 0x04);
    self->WriteData8(self, 0x13);

    self->WriteCommand(self, 0x3A);
    self->WriteData8(self, 0x05);
    self->WriteCommand(self, 0x29);

    Delay_ms(100);

    LCD_Clear(self);
}

void LCD_SetWindow(LCD_t *self, uint16_t X1, uint16_t Y1, uint16_t Width,
                   uint16_t Height) {
    uint16_t X2 = X1 + Width - 1;
    uint16_t Y2 = Y1 + Height - 1;
    if (self->Direction == 0) {
        self->WriteCommand(self, 0x2a);
        self->WriteData16(self, X1 + 2);
        self->WriteData16(self, X2 + 2);
        self->WriteCommand(self, 0x2b);
        self->WriteData16(self, Y1 + 1);
        self->WriteData16(self, Y2 + 1);
        self->WriteCommand(self, 0x2c);
    } else if (self->Direction == 1) {
        self->WriteCommand(self, 0x2a);
        self->WriteData16(self, X1 + 2);
        self->WriteData16(self, X2 + 2);
        self->WriteCommand(self, 0x2b);
        self->WriteData16(self, Y1 + 1);
        self->WriteData16(self, Y2 + 1);
        self->WriteCommand(self, 0x2c);
    } else if (self->Direction == 2) {
        self->WriteCommand(self, 0x2a);
        self->WriteData16(self, X1 + 1);
        self->WriteData16(self, X2 + 1);
        self->WriteCommand(self, 0x2b);
        self->WriteData16(self, Y1 + 2);
        self->WriteData16(self, Y2 + 2);
        self->WriteCommand(self, 0x2c);
    } else if (self->Direction == 3) {
        self->WriteCommand(self, 0x2a);
        self->WriteData16(self, X1 + 1);
        self->WriteData16(self, X2 + 1);
        self->WriteCommand(self, 0x2b);
        self->WriteData16(self, Y1 + 2);
        self->WriteData16(self, Y2 + 2);
        self->WriteCommand(self, 0x2c);
    }
}

void LCD_SetCusor(LCD_t *self, uint16_t X, uint16_t Y) {
    LCD_SetWindow(self, X, Y, self->Width - X, self->Height - Y);
}