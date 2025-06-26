#include <stdarg.h>

#include "Delay.h"
#include "OLED.h"

#include "OLED_Font.h"
#include "OLED_I2C.h"
#include "OLED_SPI.h"

void OLED_SetCursor(OLED_t *Self, uint8_t Page, uint8_t X) {
    Self->CursorCommandsBuffer[0] = 0xB0 | Page;
    Self->CursorCommandsBuffer[1] = 0x10 | ((X & 0xF0) >> 4);
    Self->CursorCommandsBuffer[2] = 0x00 | (X & 0x0F);
    Self->OLED_WriteCommands(Self, Self->CursorCommandsBuffer, 3);
}

void OLED_Fill(OLED_t *Self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < Self->Width; i++) {
            Self->DisplayBuffer[j][i] = 0xFF;
        }
    }
}

void OLED_FillArea(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                   uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < Self->Width && j >= 0 && j < Self->Height) {
                Self->DisplayBuffer[j / 8][i] |= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_Clear(OLED_t *Self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < Self->Width; i++) {
            Self->DisplayBuffer[j][i] = 0x00;
        }
    }
}

void OLED_ClearArea(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < Self->Width && j >= 0 && j < Self->Height) {
                Self->DisplayBuffer[j / 8][i] &= ~(0x01 << (j % 8));
            }
        }
    }
}

void OLED_Reverse(OLED_t *Self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < Self->Width; i++) {
            Self->DisplayBuffer[j][i] ^= 0xFF;
        }
    }
}

void OLED_ReverseArea(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                      uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < Self->Width && j >= 0 && j < Self->Height) {
                Self->DisplayBuffer[j / 8][i] ^= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_DrawPoint(OLED_t *Self, int16_t X, int16_t Y) {
    if (X >= 0 && X < Self->Width && Y >= 0 && Y < Self->Height) {
        Self->DisplayBuffer[Y / 8][X] |= 0x01 << (Y % 8);
    }
}

void OLED_DrawHLine(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t step) {
    for (uint8_t i = 0; i < Width; i += step) {
        OLED_DrawPoint(Self, X + i, Y);
    }
}

void OLED_DrawVLine(OLED_t *Self, int16_t X, int16_t Y, uint8_t Height,
                    uint8_t step) {
    for (uint8_t i = 0; i < Height; i += step) {
        OLED_DrawPoint(Self, X, Y + i);
    }
}

void OLED_DrawLine(OLED_t *Self, int16_t X1, int16_t Y1, int16_t X2,
                   int16_t Y2) {
    uint16_t dx, dy;
    if (X1 > X2) {
        dx = X1 - X2;
    } else {
        dx = X2 - X1;
    }
    if (Y1 > Y2) {
        dy = Y1 - Y2;
    } else {
        dy = Y2 - Y1;
    }

    uint16_t tmp;
    uint8_t  swapxy = 0;
    if (dy > dx) {
        swapxy = 1;
        tmp    = dx;
        dx     = dy;
        dy     = tmp;
        tmp    = X1;
        X1     = Y1;
        Y1     = tmp;
        tmp    = X2;
        X2     = Y2;
        Y2     = tmp;
    }
    if (X1 > X2) {
        tmp = X1;
        X1  = X2;
        X2  = tmp;
        tmp = Y1;
        Y1  = Y2;
        Y2  = tmp;
    }
    int16_t err   = dx >> 1;
    int16_t ystep = Y2 > Y1 ? 1 : -1;

    uint16_t y = Y1;

    if (X2 == 255) {
        X2--;
    }

    for (uint16_t x = X1; x <= X2; x++) {
        if (swapxy == 0) {
            OLED_DrawPoint(Self, x, y);
        } else {
            OLED_DrawPoint(Self, y, x);
        }
        err -= (uint16_t)dy;
        if (err < 0) {
            y += (uint16_t)ystep;
            err += (uint16_t)dx;
        }
    }
}

void OLED_DrawHollowRectangle(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                              uint8_t Height) {
    OLED_DrawHLine(Self, X, Y, Width, 1);
    OLED_DrawHLine(Self, X, Y + Height - 1, Width, 1);
    OLED_DrawVLine(Self, X, Y, Height, 1);
    OLED_DrawVLine(Self, X + Width - 1, Y, Height, 1);
}

void OLED_ShowChart(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, uint16_t *Data, uint16_t Length,
                    int16_t Index) {
    Index = (Index + 1) % Length;
    for (uint8_t x = 0; x < Self->Width - 1;
         x++, Index = (Index + 1) % Length) {
        OLED_DrawLine(Self, OLED_IndexToX(x, Length, X, Width),
                      OLED_ADCToY(Data[Index], Y, Height),
                      OLED_IndexToX(x + 1, Length, X, Width),
                      OLED_ADCToY(Data[(Index + 1) % Length], Y, Height));
    }

    OLED_DrawHLine(Self, X, Y, Width, 3);
    OLED_DrawHLine(Self, X, Y + Height - 1, Width, 3);
    OLED_DrawVLine(Self, X, Y, Height, 3);
    OLED_DrawVLine(Self, X + Width - 1, Y, Height, 3);
}

void OLED_ShowImage(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, const uint8_t *Image) {
    OLED_ClearArea(Self, X, Y, Width, Height);

    for (uint8_t j = 0; j < (Height - 1) / 8 + 1; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i < Self->Width) {
                int16_t Page  = Y / 8;
                int16_t Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }

                if (Page + j >= 0 && Page + j <= 7) {
                    Self->DisplayBuffer[Page + j][X + i] |= Image[j * Width + i]
                                                            << (Shift);
                }

                if (Page + j + 1 >= 0 && Page + j + 1 <= 7) {
                    Self->DisplayBuffer[Page + j + 1][X + i] |=
                        Image[j * Width + i] >> (8 - Shift);
                }
            }
        }
    }
}

void OLED_SetFont(OLED_t *Self, OLEDFont Font) {
    Self->Font = Font;

    switch (Self->Font) {
    case OLEDFont_6X8:
        Self->FontWidth  = 6;
        Self->FontHeight = 8;
        break;

    case OLEDFont_8X16:
        Self->FontWidth  = 8;
        Self->FontHeight = 16;
        break;

    case OLEDFont_Chinese12X12:
        Self->FontWidth  = 12;
        Self->FontHeight = 12;
        break;
    }
}

void OLED_ShowChar(OLED_t *Self, int16_t X, int16_t Y, char Char) {
    switch (Self->Font) {
    case OLEDFont_6X8:
        OLED_ShowImage(Self, X, Y, Self->FontWidth, Self->FontHeight,
                       OLED_Font6x8[Char - ' ']);
        break;

    case OLEDFont_8X16:
        OLED_ShowImage(Self, X, Y, Self->FontWidth, Self->FontHeight,
                       OLED_Font8x16[Char - ' ']);
        break;

    default:
        break;
    }
}

void OLED_ShowString(OLED_t *Self, int16_t X, int16_t Y, const char *String) {
    uint8_t ChineseFontLength =
        strlen(OLED_FontChinese12x12_Index) / OLED_ChineseBytesCount;

    for (uint8_t i = 0; String[i];) {

        if (IsChinese(String + i)) {
            uint8_t Index = 0;
            while (strncmp((char *)&String[i],
                           &OLED_FontChinese12x12_Index[Index *
                                                        OLED_ChineseBytesCount],
                           OLED_ChineseBytesCount) != 0 &&
                   ++Index < ChineseFontLength) {
            }

            if (Index == ChineseFontLength) {
                OLEDFont Font = Self->Font;
                OLED_SetFont(Self, OLEDFont_8X16);
                OLED_ShowChar(Self, X, Y, '?');
                X += Self->FontWidth;
                OLED_SetFont(Self, Font);

            } else {
                OLEDFont Font = Self->Font;
                OLED_SetFont(Self, OLEDFont_Chinese12X12);
                OLED_ShowImage(Self, X, Y, Self->FontWidth, Self->FontHeight,
                               OLED_FontChinese12x12[Index]);
                X += Self->FontWidth;
                OLED_SetFont(Self, Font);
            }

            i += OLED_ChineseBytesCount;

        } else {
            OLED_ShowChar(Self, X, Y, String[i]);
            X += Self->FontWidth;

            i += 1;
        }
    }
}

void OLED_Printf(OLED_t *Self, int16_t x, int16_t y, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)Self->PrintfBuffer, format, arg);
    va_end(arg);

    OLED_ShowString(Self, x, y, (char *)Self->PrintfBuffer);
}

void OLED_ClearBuffer(OLED_t *Self) { OLED_Clear(Self); }

void OLED_ClearBufferArea(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                          uint8_t Height) {
    OLED_ClearArea(Self, X, Y, Width, Height);
}

void OLED_SendBuffer(OLED_t *Self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(Self, j, 0);
        Self->OLED_WriteDatas(Self, Self->DisplayBuffer[j], Self->Width);
    }
}

void OLED_SendBufferArea(OLED_t *Self, int16_t X, int16_t Y, uint8_t Width,
                         uint8_t Height) {
    int16_t Page  = Y / 8;
    int16_t Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) {
        Page -= 1;
        Page1 -= 1;
    }

    for (int16_t j = Page; j < Page1; j++) {
        if (X >= 0 && X < Self->Width && j >= 0 && j <= 7) {

            OLED_SetCursor(Self, j, X);
            Self->OLED_WriteDatas(Self, &Self->DisplayBuffer[j][X], Width);
        }
    }
}

void OLED_Init(OLED_t *Self) {
    if (Self->I2C || Self->I2Cx) {
        GPIO_t GPIO;
        GPIO.Mode     = Self->I2Cx ? GPIO_Mode_AF_OD : GPIO_Mode_Out_OD;
        Self->SCL_ODR = GPIO_InitPin(&GPIO, Self->SCL);
        Self->SDA_ODR = GPIO_InitPin(&GPIO, Self->SDA);

        if (Self->I2C) {
            GPIO_Write(Self->SCL_ODR, 1);
            GPIO_Write(Self->SDA_ODR, 1);

            Self->OLED_WriteDatas    = OLED_SWI2C_WriteDatas;
            Self->OLED_WriteCommand  = OLED_SWI2C_WriteCommand;
            Self->OLED_WriteCommands = OLED_SWI2C_WriteCommands;
        } else if (Self->I2Cx) {
            Self->OLED_WriteDatas    = OLED_HWI2C_WriteDatas;
            Self->OLED_WriteCommand  = OLED_HWI2C_WriteCommand;
            Self->OLED_WriteCommands = OLED_HWI2C_WriteCommands;
        }

        if (Self->I2Cx) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            I2C_InitTypeDef I2C_InitStructure;
            I2C_InitStructure.I2C_Mode        = I2C_Mode_I2C;
            I2C_InitStructure.I2C_DutyCycle   = I2C_DutyCycle_16_9;
            I2C_InitStructure.I2C_OwnAddress1 = 0x00;
            I2C_InitStructure.I2C_Ack         = I2C_Ack_Disable;
            I2C_InitStructure.I2C_AcknowledgedAddress =
                I2C_AcknowledgedAddress_7bit;
            I2C_InitStructure.I2C_ClockSpeed = 400000;
            I2C_Init(Self->I2Cx, &I2C_InitStructure);

            I2C_Cmd(Self->I2Cx, ENABLE);
        }
    } else if (Self->SPI || Self->SPIx) {
        GPIO_t GPIO;
        GPIO.Mode     = Self->SPIx ? GPIO_Mode_AF_PP : GPIO_Mode_Out_PP;
        Self->D0_ODR  = GPIO_InitPin(&GPIO, Self->D0);
        Self->D1_ODR  = GPIO_InitPin(&GPIO, Self->D1);
        GPIO.Mode     = GPIO_Mode_Out_PP;
        Self->RES_ODR = GPIO_InitPin(&GPIO, Self->RES);
        Self->DC_ODR  = GPIO_InitPin(&GPIO, Self->DC);
        Self->CS_ODR  = GPIO_InitPin(&GPIO, Self->CS);

        if (Self->SPI) {
            GPIO_Write(Self->D0, 1);
            GPIO_Write(Self->D1, 1);
            GPIO_Write(Self->RES_ODR, 1);
            GPIO_Write(Self->DC_ODR, 1);
            GPIO_Write(Self->CS_ODR, 1);

            Self->OLED_WriteDatas    = OLED_SWSPI_WriteDatas;
            Self->OLED_WriteCommand  = OLED_SWSPI_WriteCommand;
            Self->OLED_WriteCommands = OLED_SWSPI_WriteCommands;

        } else if (Self->SPIx) {
            GPIO_Write(Self->RES_ODR, 1);
            GPIO_Write(Self->DC_ODR, 1);
            GPIO_Write(Self->CS_ODR, 1);

            Self->OLED_WriteDatas    = OLED_HWSPI_WriteDatas;
            Self->OLED_WriteCommand  = OLED_HWSPI_WriteCommand;
            Self->OLED_WriteCommands = OLED_HWSPI_WriteCommands;
        }

        if (Self->SPIx) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            SPI_InitTypeDef SPI_InitStructure;
            SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
            SPI_InitStructure.SPI_Direction         = SPI_Direction_1Line_Tx;
            SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
            SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
            SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
            SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
            SPI_InitStructure.SPI_CRCPolynomial     = 7;
            SPI_Init(Self->SPIx, &SPI_InitStructure);

            SPI_Cmd(Self->SPIx, ENABLE);
        }
    }

    if (Self->SPI || Self->SPIx) {
        GPIO_Write(Self->RES_ODR, 0);
    }
    Delay_ms(100);
    if (Self->SPI || Self->SPIx) {
        GPIO_Write(Self->RES_ODR, 1);
    }

    Self->OLED_WriteCommand(Self, 0xAE); // 关闭显示

    Self->OLED_WriteCommand(Self, 0xD5); // 设置时钟
    Self->OLED_WriteCommand(Self, 0x80);

    Self->OLED_WriteCommand(Self, 0xA8); // 设置多路复
    Self->OLED_WriteCommand(Self, 0x3F);

    Self->OLED_WriteCommand(Self, 0xD3); // 设置显示偏移
    Self->OLED_WriteCommand(Self, 0x00);

    Self->OLED_WriteCommand(Self, 0x40); // 设置起始行

    Self->OLED_WriteCommand(Self, 0xA1); // 设置左右方向

    Self->OLED_WriteCommand(Self, 0xC8); // 设置上下方向

    Self->OLED_WriteCommand(Self, 0xDA); // 设置COM硬件引脚配置
    Self->OLED_WriteCommand(Self, 0x12);

    Self->OLED_WriteCommand(Self, 0x81); // 调节亮度
    Self->OLED_WriteCommand(Self, 0xCF);

    Self->OLED_WriteCommand(Self, 0xD9); // 设置预充电周期
    Self->OLED_WriteCommand(Self, 0xF1);

    Self->OLED_WriteCommand(Self, 0xDB); // 设置VCOMH
    Self->OLED_WriteCommand(Self, 0x30);

    Self->OLED_WriteCommand(Self, 0xA4); // 全局显示开启

    Self->OLED_WriteCommand(Self, 0xA6); // 设置显示方式

    Self->OLED_WriteCommand(Self, 0x8D); // 设置电荷泵
    Self->OLED_WriteCommand(Self, 0x14);

    Self->OLED_WriteCommand(Self, 0xAF); // 打开显示

    OLED_Clear(Self);
}
