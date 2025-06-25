#include <stdarg.h>

#include "Delay.h"
#include "LCD.h"
#include "LCD_SPI.h"

void LCD_SetWindow(LCD_t *Self, uint16_t X1, uint16_t Y1, uint16_t Width,
                   uint16_t Height);
void LCD_SetCusor(LCD_t *Self, uint16_t X, uint16_t Y);

void LCD_SetPenColor(LCD_t *Self, uint16_t Color) { Self->PenColor = Color; }

void LCD_SetBackColor(LCD_t *Self, uint16_t Color) { Self->BackColor = Color; }

void LCD_SetFont(LCD_t *Self, uint32_t Font) {
    Self->Font = Font;
    for (uint8_t i = 0; i < sizeof(LCDFonts) / sizeof(LCDFonts[0]); i++) {
        if (LCDFonts[i].Font == Self->Font) {
            Self->FontWidth  = LCDFonts[i].FontWidth;
            Self->FontHeight = LCDFonts[i].FontHeight;
            break;
        }
    }
}

void LCD_Fill(LCD_t *Self, uint16_t X, uint16_t Y, uint16_t Width,
              uint16_t Height, uint16_t Color) {
    LCD_SetWindow(Self, X, Y, Width, Height);
    Self->WriteData16s(Self, Color, Width * Height);
}

void LCD_Clear(LCD_t *Self) {
    LCD_Fill(Self, 0, 0, Self->Width, Self->Height, Self->BackColor);
}

void LCD_DrawPoint(LCD_t *Self, uint16_t X, uint16_t Y) {
    LCD_SetCusor(Self, X, Y);
    Self->WriteData16(Self, Self->PenColor);
}

void LCD_DrawHLine(LCD_t *Self, uint16_t X, uint16_t Y, uint16_t Length) {
    LCD_SetWindow(Self, X, Y, Length, 1);
    Self->WriteData16s(Self, Self->PenColor, Length);
}

void LCD_DrawVLine(LCD_t *Self, uint16_t X, uint16_t Y, uint16_t Length) {
    LCD_SetWindow(Self, X, Y, 1, Length);
    Self->WriteData16s(Self, Self->PenColor, Length);
}

void LCD_ShowChar(LCD_t *Self, uint16_t X, uint16_t Y, uint8_t Char) {
    LCD_SetWindow(Self, X, Y, Self->FontWidth, Self->FontHeight);

    uint8_t bytePerLine = (Self->FontWidth + 7) / 8;
    for (uint16_t Line = 0; Line < Self->FontHeight; Line++) {
        for (uint16_t Column = 0; Column < Self->FontWidth; Column++) {
            if (Self->Font == (uint32_t)LCD_Font8x16) {
                if (LCD_Font8x16[Char - ' '][Line * bytePerLine + Column / 8] &
                    (0b10000000 >> (Column % 8))) {
                    Self->WriteData16(Self, Self->PenColor);

                } else {
                    Self->WriteData16(Self, Self->BackColor);
                }
            }
        }
    }
}

void LCD_ShowString(LCD_t *Self, uint16_t X, uint16_t Y, char *String) {
    for (uint8_t i = 0; String[i]; i++) {
        LCD_ShowChar(Self, X, Y, String[i]);
        X += Self->FontWidth;
    }
}

void LCD_Printf(LCD_t *Self, int16_t X, int16_t Y, const char *Format, ...) {
    va_list arg;
    va_start(arg, Format);
    vsprintf((char *)Self->PrintfBuffer, Format, arg);
    va_end(arg);

    LCD_ShowString(Self, X, Y, (char *)Self->PrintfBuffer);
}

void LCD_ShowImage(LCD_t *Self, uint16_t X, uint16_t Y, uint16_t Width,
                   uint16_t Height, const uint8_t *Image) {
    LCD_SetWindow(Self, X, Y, Width, Height);

    Self->WriteDatas16(Self, (uint16_t *)Image, Width * Height);
}

void LCD_Init(LCD_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;

    Self->SCL_ODR = GPIO_InitPin(&GPIO, Self->SCL);
    Self->SDA_ODR = GPIO_InitPin(&GPIO, Self->SDA);
    Self->RES_ODR = GPIO_InitPin(&GPIO, Self->RES);
    Self->DC_ODR  = GPIO_InitPin(&GPIO, Self->DC);
    Self->CS_ODR  = GPIO_InitPin(&GPIO, Self->CS);
    Self->BL_ODR  = GPIO_InitPin(&GPIO, Self->BL);

    GPIO_Write(Self->SCL_ODR, 1);
    GPIO_Write(Self->SDA_ODR, 1);
    GPIO_Write(Self->RES_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
    GPIO_Write(Self->CS_ODR, 1);
    GPIO_Write(Self->BL_ODR, 0);

    Self->WriteData8   = LCD_SWSPI_WriteData8;
    Self->WriteData16  = LCD_SWSPI_WriteData16;
    Self->WriteData16s = LCD_SWSPI_WriteData16s;
    Self->WriteDatas16 = LCD_SWSPI_WriteDatas16;
    Self->WriteCommand = LCD_SWSPI_WriteCommand;

    GPIO_Write(Self->RES_ODR, 0);
    Delay_ms(100);
    GPIO_Write(Self->RES_ODR, 1);
    Delay_ms(100);

    GPIO_Write(Self->BL_ODR, 1);
    Delay_ms(100);

    Self->WriteCommand(Self, 0x11);
    Delay_ms(120);

    Self->WriteCommand(Self, 0xB1);
    Self->WriteData8(Self, 0x05);
    Self->WriteData8(Self, 0x3C);
    Self->WriteData8(Self, 0x3C);
    Self->WriteCommand(Self, 0xB2);
    Self->WriteData8(Self, 0x05);
    Self->WriteData8(Self, 0x3C);
    Self->WriteData8(Self, 0x3C);
    Self->WriteCommand(Self, 0xB3);
    Self->WriteData8(Self, 0x05);
    Self->WriteData8(Self, 0x3C);
    Self->WriteData8(Self, 0x3C);
    Self->WriteData8(Self, 0x05);
    Self->WriteData8(Self, 0x3C);
    Self->WriteData8(Self, 0x3C);

    Self->WriteCommand(Self, 0xB4);
    Self->WriteData8(Self, 0x03);

    Self->WriteCommand(Self, 0xC0);
    Self->WriteData8(Self, 0x28);
    Self->WriteData8(Self, 0x08);
    Self->WriteData8(Self, 0x04);
    Self->WriteCommand(Self, 0xC1);
    Self->WriteData8(Self, 0XC0);
    Self->WriteCommand(Self, 0xC2);
    Self->WriteData8(Self, 0x0D);
    Self->WriteData8(Self, 0x00);
    Self->WriteCommand(Self, 0xC3);
    Self->WriteData8(Self, 0x8D);
    Self->WriteData8(Self, 0x2A);
    Self->WriteCommand(Self, 0xC4);
    Self->WriteData8(Self, 0x8D);
    Self->WriteData8(Self, 0xEE);

    Self->WriteCommand(Self, 0xC5);
    Self->WriteData8(Self, 0x1A);
    Self->WriteCommand(Self, 0x36);
    if (Self->Direction == 0) {
        Self->WriteData8(Self, 0x00);

    } else if (Self->Direction == 1) {
        Self->WriteData8(Self, 0xC0);

    } else if (Self->Direction == 2) {
        Self->WriteData8(Self, 0x70);

    } else if (Self->Direction == 3) {
        Self->WriteData8(Self, 0xA0);
    }

    Self->WriteCommand(Self, 0xE0);
    Self->WriteData8(Self, 0x04);
    Self->WriteData8(Self, 0x22);
    Self->WriteData8(Self, 0x07);
    Self->WriteData8(Self, 0x0A);
    Self->WriteData8(Self, 0x2E);
    Self->WriteData8(Self, 0x30);
    Self->WriteData8(Self, 0x25);
    Self->WriteData8(Self, 0x2A);
    Self->WriteData8(Self, 0x28);
    Self->WriteData8(Self, 0x26);
    Self->WriteData8(Self, 0x2E);
    Self->WriteData8(Self, 0x3A);
    Self->WriteData8(Self, 0x00);
    Self->WriteData8(Self, 0x01);
    Self->WriteData8(Self, 0x03);
    Self->WriteData8(Self, 0x13);
    Self->WriteCommand(Self, 0xE1);
    Self->WriteData8(Self, 0x04);
    Self->WriteData8(Self, 0x16);
    Self->WriteData8(Self, 0x06);
    Self->WriteData8(Self, 0x0D);
    Self->WriteData8(Self, 0x2D);
    Self->WriteData8(Self, 0x26);
    Self->WriteData8(Self, 0x23);
    Self->WriteData8(Self, 0x27);
    Self->WriteData8(Self, 0x27);
    Self->WriteData8(Self, 0x25);
    Self->WriteData8(Self, 0x2D);
    Self->WriteData8(Self, 0x3B);
    Self->WriteData8(Self, 0x00);
    Self->WriteData8(Self, 0x01);
    Self->WriteData8(Self, 0x04);
    Self->WriteData8(Self, 0x13);

    Self->WriteCommand(Self, 0x3A);
    Self->WriteData8(Self, 0x05);
    Self->WriteCommand(Self, 0x29);

    Delay_ms(100);

    LCD_Clear(Self);
}

void LCD_SetWindow(LCD_t *Self, uint16_t X1, uint16_t Y1, uint16_t Width,
                   uint16_t Height) {
    uint16_t X2 = X1 + Width - 1;
    uint16_t Y2 = Y1 + Height - 1;
    if (Self->Direction == 0) {
        Self->WriteCommand(Self, 0x2a);
        Self->WriteData16(Self, X1 + 2);
        Self->WriteData16(Self, X2 + 2);
        Self->WriteCommand(Self, 0x2b);
        Self->WriteData16(Self, Y1 + 1);
        Self->WriteData16(Self, Y2 + 1);
        Self->WriteCommand(Self, 0x2c);
    } else if (Self->Direction == 1) {
        Self->WriteCommand(Self, 0x2a);
        Self->WriteData16(Self, X1 + 2);
        Self->WriteData16(Self, X2 + 2);
        Self->WriteCommand(Self, 0x2b);
        Self->WriteData16(Self, Y1 + 1);
        Self->WriteData16(Self, Y2 + 1);
        Self->WriteCommand(Self, 0x2c);
    } else if (Self->Direction == 2) {
        Self->WriteCommand(Self, 0x2a);
        Self->WriteData16(Self, X1 + 1);
        Self->WriteData16(Self, X2 + 1);
        Self->WriteCommand(Self, 0x2b);
        Self->WriteData16(Self, Y1 + 2);
        Self->WriteData16(Self, Y2 + 2);
        Self->WriteCommand(Self, 0x2c);
    } else if (Self->Direction == 3) {
        Self->WriteCommand(Self, 0x2a);
        Self->WriteData16(Self, X1 + 1);
        Self->WriteData16(Self, X2 + 1);
        Self->WriteCommand(Self, 0x2b);
        Self->WriteData16(Self, Y1 + 2);
        Self->WriteData16(Self, Y2 + 2);
        Self->WriteCommand(Self, 0x2c);
    }
}

void LCD_SetCusor(LCD_t *Self, uint16_t X, uint16_t Y) {
    LCD_SetWindow(Self, X, Y, Self->Width - X, Self->Height - Y);
}