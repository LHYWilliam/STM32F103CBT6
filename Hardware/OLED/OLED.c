#include <stdarg.h>

#include "Delay.h"
#include "GPIO.h"
#include "OLED.h"

#if U8G2

uint32_t SCL_ODR;
uint32_t SDA_ODR;
I2C_TypeDef *I2Cx;
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr);
uint8_t u8g2_gpio_and_delay_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                void *arg_ptr);

uint32_t D0_ODR;
uint32_t D1_ODR;
uint32_t RES_ODR;
uint32_t DC_ODR;
uint32_t CS_ODR;
SPI_TypeDef *SPIx;
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                               void *arg_ptr);
uint8_t u8g2_gpio_and_delay_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                void *arg_ptr);

#endif

#include "OLED_Font.h"
#include "OLED_I2C.h"
#include "OLED_SPI.h"

void OLED_Init(OLED_t *self) {
    if (self->I2C || self->I2Cx) {
        GPIO_t GPIO;
        GPIO.Mode = self->I2Cx ? GPIO_Mode_AF_OD : GPIO_Mode_Out_OD;
        GPIO_InitPin(&GPIO, self->SCL);
        GPIO_InitPin(&GPIO, self->SDA);

        if (self->I2C) {
#if U8G2
            if (self->U8g2) {
                SCL_ODR = GPIO_ODR(self->SCL);
                SDA_ODR = GPIO_ODR(self->SDA);

                GPIO_Write(SCL_ODR, 1);
                GPIO_Write(SDA_ODR, 1);

            } else {
#endif
                self->SCL_ODR = GPIO_ODR(self->SCL);
                self->SDA_ODR = GPIO_ODR(self->SDA);

                GPIO_Write(self->SCL_ODR, 1);
                GPIO_Write(self->SDA_ODR, 1);

                self->OLED_WriteData = OLED_SWI2C_WriteData;
                self->OLED_WriteDatas = OLED_SWI2C_WriteDatas;
                self->OLED_WriteCommand = OLED_SWI2C_WriteCommand;
                self->OLED_WriteCommands = OLED_SWI2C_WriteCommands;
#if U8G2
            }
#endif
        } else if (self->I2Cx) {
#if U8G2
            if (!self->U8g2) {
#endif
                self->OLED_WriteData = OLED_HWI2C_WriteData;
                self->OLED_WriteDatas = OLED_HWI2C_WriteDatas;
                self->OLED_WriteCommand = OLED_HWI2C_WriteCommand;
                self->OLED_WriteCommands = OLED_HWI2C_WriteCommands;
#if U8G2
            }
#endif
        }

        if (self->I2Cx) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            I2C_InitTypeDef I2C_InitStructure;
            I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
            I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
            I2C_InitStructure.I2C_OwnAddress1 = 0x00;
            I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
            I2C_InitStructure.I2C_AcknowledgedAddress =
                I2C_AcknowledgedAddress_7bit;
            I2C_InitStructure.I2C_ClockSpeed = 400000;
            I2C_Init(self->I2Cx, &I2C_InitStructure);

            I2C_Cmd(self->I2Cx, ENABLE);
#if U8G2
            if (self->U8g2) {
                I2Cx = self->I2Cx;
            }
#endif
        }

    } else if (self->SPI || self->SPIx) {
        GPIO_t GPIO;
        GPIO.Mode = self->SPIx ? GPIO_Mode_AF_PP : GPIO_Mode_Out_PP;
        GPIO_InitPin(&GPIO, self->D0);
        GPIO_InitPin(&GPIO, self->D1);
        GPIO.Mode = GPIO_Mode_Out_PP;
        GPIO_InitPin(&GPIO, self->RES);
        GPIO_InitPin(&GPIO, self->DC);
        GPIO_InitPin(&GPIO, self->CS);

        if (self->SPI) {
#if U8G2
            if (self->U8g2) {
                D0_ODR = GPIO_ODR(self->D0);
                D1_ODR = GPIO_ODR(self->D1);
                RES_ODR = GPIO_ODR(self->RES);
                DC_ODR = GPIO_ODR(self->DC);
                CS_ODR = GPIO_ODR(self->CS);

                GPIO_Write(RES_ODR, 1);
                GPIO_Write(DC_ODR, 1);
                GPIO_Write(CS_ODR, 1);

            } else {
#endif
                self->D0_ODR = GPIO_ODR(self->D0);
                self->D1_ODR = GPIO_ODR(self->D1);
                self->RES_ODR = GPIO_ODR(self->RES);
                self->DC_ODR = GPIO_ODR(self->DC);
                self->CS_ODR = GPIO_ODR(self->CS);

                GPIO_Write(self->D0, 1);
                GPIO_Write(self->D1, 1);
                GPIO_Write(self->RES_ODR, 1);
                GPIO_Write(self->DC_ODR, 1);
                GPIO_Write(self->CS_ODR, 1);

                self->OLED_WriteData = OLED_SWSPI_WriteData;
                self->OLED_WriteDatas = OLED_SWSPI_WriteDatas;
                self->OLED_WriteCommand = OLED_SWSPI_WriteCommand;
                self->OLED_WriteCommands = OLED_SWSPI_WriteCommands;
#if U8G2
            }
#endif
        } else if (self->SPIx) {
#if U8G2
            if (self->U8g2) {
                RES_ODR = GPIO_ODR(self->RES);
                DC_ODR = GPIO_ODR(self->DC);
                CS_ODR = GPIO_ODR(self->CS);

                GPIO_Write(RES_ODR, 1);
                GPIO_Write(DC_ODR, 1);
                GPIO_Write(CS_ODR, 1);
            } else {
#endif
                self->RES_ODR = GPIO_ODR(self->RES);
                self->DC_ODR = GPIO_ODR(self->DC);
                self->CS_ODR = GPIO_ODR(self->CS);

                GPIO_Write(self->RES_ODR, 1);
                GPIO_Write(self->DC_ODR, 1);
                GPIO_Write(self->CS_ODR, 1);

                self->OLED_WriteData = OLED_HWSPI_WriteData;
                self->OLED_WriteDatas = OLED_HWSPI_WriteDatas;
                self->OLED_WriteCommand = OLED_HWSPI_WriteCommand;
                self->OLED_WriteCommands = OLED_HWSPI_WriteCommands;
#if U8G2
            }
#endif
        }

        if (self->SPIx) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            SPI_InitTypeDef SPI_InitStructure;
            SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
            SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
            SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
            SPI_InitStructure.SPI_CRCPolynomial = 7;
            SPI_Init(self->SPIx, &SPI_InitStructure);

            SPI_Cmd(self->SPIx, ENABLE);
#if U8G2
            if (self->U8g2) {
                SPIx = self->SPIx;
            }
#endif
        }
    }

#if U8G2
    if (self->U8g2) {
        if (self->I2Cx) {
            Delay_ms(100);

            u8g2_Setup_ssd1306_i2c_128x64_noname_f(&self->u8g2, U8G2_R0,
                                                   u8x8_byte_hw_i2c,
                                                   u8g2_gpio_and_delay_i2c);
        } else if (self->I2C) {
            Delay_ms(100);

            u8g2_Setup_ssd1306_i2c_128x64_noname_f(&self->u8g2, U8G2_R0,
                                                   u8x8_byte_sw_i2c,
                                                   u8g2_gpio_and_delay_i2c);
        } else if (self->SPIx) {
            GPIO_Write(RES_ODR, 0);
            Delay_ms(100);
            GPIO_Write(RES_ODR, 1);

            u8g2_Setup_ssd1306_128x64_noname_f(&self->u8g2, U8G2_R0,
                                               u8x8_byte_4wire_hw_spi,
                                               u8g2_gpio_and_delay_spi);
        } else if (self->SPI) {
            GPIO_Write(RES_ODR, 0);
            Delay_ms(100);
            GPIO_Write(RES_ODR, 1);

            u8g2_Setup_ssd1306_128x64_noname_f(&self->u8g2, U8G2_R0,
                                               u8x8_byte_4wire_sw_spi,
                                               u8g2_gpio_and_delay_spi);
        }

        u8g2_InitDisplay(&self->u8g2);
        u8g2_SetPowerSave(&self->u8g2, 0);

    } else {
#endif
        if (self->SPI || self->SPIx) {
            GPIO_Write(self->RES_ODR, 0);
        }
        Delay_ms(100);
        if (self->SPI || self->SPIx) {
            GPIO_Write(self->RES_ODR, 1);
        }

        self->OLED_WriteCommand(self, 0xAE); // 关闭显示

        self->OLED_WriteCommand(self, 0xD5); // 设置时钟
        self->OLED_WriteCommand(self, 0x80);

        self->OLED_WriteCommand(self, 0xA8); // 设置多路复
        self->OLED_WriteCommand(self, 0x3F);

        self->OLED_WriteCommand(self, 0xD3); // 设置显示偏移
        self->OLED_WriteCommand(self, 0x00);

        self->OLED_WriteCommand(self, 0x40); // 设置起始行

        self->OLED_WriteCommand(self, 0xA1); // 设置左右方向

        self->OLED_WriteCommand(self, 0xC8); // 设置上下方向

        self->OLED_WriteCommand(self, 0xDA); // 设置COM硬件引脚配置
        self->OLED_WriteCommand(self, 0x12);

        self->OLED_WriteCommand(self, 0x81); // 调节亮度
        self->OLED_WriteCommand(self, 0xCF);

        self->OLED_WriteCommand(self, 0xD9); // 设置预充电周期
        self->OLED_WriteCommand(self, 0xF1);

        self->OLED_WriteCommand(self, 0xDB); // 设置VCOMH
        self->OLED_WriteCommand(self, 0x30);

        self->OLED_WriteCommand(self, 0xA4); // 全局显示开启

        self->OLED_WriteCommand(self, 0xA6); // 设置显示方式

        self->OLED_WriteCommand(self, 0x8D); // 设置电荷泵
        self->OLED_WriteCommand(self, 0x14);

        self->OLED_WriteCommand(self, 0xAF); // 打开显示

        OLED_Clear(self);

#if U8G2
    }
#endif
}

void OLED_SetCursor(OLED_t *self, uint8_t Page, uint8_t X) {
    self->CursorCommandsBuffer[0] = 0xB0 | Page;
    self->CursorCommandsBuffer[1] = 0x10 | ((X & 0xF0) >> 4);
    self->CursorCommandsBuffer[2] = 0x00 | (X & 0x0F);
    self->OLED_WriteCommands(self, self->CursorCommandsBuffer, 3);
}

void OLED_Fill(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] = 0xFF;
        }
    }
}

void OLED_FillArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                   uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] |= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_Clear(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] = 0x00;
        }
    }
}

void OLED_ClearArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] &= ~(0x01 << (j % 8));
            }
        }
    }
}

void OLED_Reverse(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] ^= 0xFF;
        }
    }
}

void OLED_ReverseArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                      uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] ^= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_DrawPoint(OLED_t *self, int16_t X, int16_t Y) {
    if (X >= 0 && X < self->Width && Y >= 0 && Y < self->Height) {
        self->DisplayBuffer[Y / 8][X] |= 0x01 << (Y % 8);
    }
}

void OLED_DrawLine(OLED_t *self, int16_t X1, int16_t Y1, int16_t X2,
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
    uint8_t swapxy = 0;
    if (dy > dx) {
        swapxy = 1;
        tmp = dx;
        dx = dy;
        dy = tmp;
        tmp = X1;
        X1 = Y1;
        Y1 = tmp;
        tmp = X2;
        X2 = Y2;
        Y2 = tmp;
    }
    if (X1 > X2) {
        tmp = X1;
        X1 = X2;
        X2 = tmp;
        tmp = Y1;
        Y1 = Y2;
        Y2 = tmp;
    }
    int16_t err = dx >> 1;
    int16_t ystep = Y2 > Y1 ? 1 : -1;

    uint16_t y = Y1;

    if (X2 == 255) {
        X2--;
    }

    for (uint16_t x = X1; x <= X2; x++) {
        if (swapxy == 0) {
            OLED_DrawPoint(self, x, y);
        } else {
            OLED_DrawPoint(self, y, x);
        }
        err -= (uint16_t)dy;
        if (err < 0) {
            y += (uint16_t)ystep;
            err += (uint16_t)dx;
        }
    }
}

void OLED_ShowImage(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, const uint8_t *Image) {
    for (uint8_t j = 0; j < (Height - 1) / 8 + 1; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i < self->Width) {
                int16_t Page = Y / 8;
                int16_t Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }

                if (Page + j >= 0 && Page + j <= 7) {
                    self->DisplayBuffer[Page + j][X + i] |= Image[j * Width + i]
                                                            << (Shift);
                }

                if (Page + j + 1 >= 0 && Page + j + 1 <= 7) {
                    self->DisplayBuffer[Page + j + 1][X + i] |=
                        Image[j * Width + i] >> (8 - Shift);
                }
            }
        }
    }
}

void OLED_SetFont(OLED_t *self, const uint8_t *Font) { self->Font = Font; }

void OLED_ShowChar(OLED_t *self, int16_t X, int16_t Y, char Char) {
    OLED_ShowImage(self, X, Y, 8, 16, &self->Font[(Char - ' ') * 16]);
}

void OLED_ShowString(OLED_t *self, int16_t X, int16_t Y, const char *String) {
    for (uint8_t i = 0; String[i]; i++) {
        OLED_ShowChar(self, X + i * 8, Y, String[i]);
    }
}

void OLED_Printf(OLED_t *self, int16_t x, int16_t y, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->PrintfBuffer, format, arg);
    va_end(arg);

    OLED_ShowString(self, x, y, (char *)self->PrintfBuffer);
}

void OLED_ClearBuffer(OLED_t *self) { OLED_Clear(self); }

void OLED_ClearBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                          uint8_t Height) {
    OLED_ClearArea(self, X, Y, Width, Height);
}

void OLED_SendBuffer(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(self, j, 0);
        self->OLED_WriteDatas(self, self->DisplayBuffer[j], self->Width);
    }
}

void OLED_SendBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                         uint8_t Height) {
    int16_t Page = Y / 8;
    int16_t Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) {
        Page -= 1;
        Page1 -= 1;
    }

    for (int16_t j = Page; j < Page1; j++) {
        if (X >= 0 && X < self->Width && j >= 0 && j <= 7) {

            OLED_SetCursor(self, j, X);
            self->OLED_WriteDatas(self, &self->DisplayBuffer[j][X], Width);
        }
    }
}

#if U8G2

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_BYTE_START_TRANSFER:
        I2C_GenerateSTART(I2Cx, ENABLE);
        while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
            ;

        I2C_Send7bitAddress(I2Cx, 0x78, I2C_Direction_Transmitter);
        while (I2C_CheckEvent(
                   I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
            ;
        break;

    case U8X8_MSG_BYTE_SEND:
        for (uint8_t i = 0; i < arg_int; i++) {
            I2C_SendData(I2Cx, ((uint8_t *)arg_ptr)[i]);
            while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
                   SUCCESS)
                ;
        }
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        I2C_GenerateSTOP(I2Cx, ENABLE);
        break;

    default:
        break;
    }

    return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                               void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_BYTE_SET_DC:
        GPIO_Write(DC_ODR, arg_int);
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        GPIO_Write(CS_ODR, 0);
        break;

    case U8X8_MSG_BYTE_SEND:
        for (uint8_t i = 0; i < arg_int; i++) {
            while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) != SET)
                ;
            SPI_I2S_SendData(SPIx, ((uint8_t *)arg_ptr)[i]);
        }
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        GPIO_Write(CS_ODR, 1);
        break;

    default:
        break;
    }

    return 1;
}

uint8_t u8g2_gpio_and_delay_i2c(U8X8_UNUSED u8x8_t *u8x8,
                                U8X8_UNUSED uint8_t msg,
                                U8X8_UNUSED uint8_t arg_int,
                                U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_GPIO_I2C_CLOCK:
        GPIO_Write(SCL_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_I2C_DATA:
        GPIO_Write(SDA_ODR, arg_int);
        break;

    case U8X8_MSG_DELAY_MILLI:
        Delay_ms(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        Delay_us(10);
        break;

    case U8X8_MSG_DELAY_100NANO:
        __NOP();
        break;

    default:
        break;
    }

    return 1;
}

uint8_t u8g2_gpio_and_delay_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_GPIO_SPI_CLOCK:
        GPIO_Write(D0_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_SPI_DATA:
        GPIO_Write(D1_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_CS:
        GPIO_Write(CS_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_DC:
        GPIO_Write(DC_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_RESET:
        GPIO_Write(RES_ODR, arg_int);
        break;

    case U8X8_MSG_DELAY_MILLI:
        Delay_ms(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        Delay_us(10);
        break;

    case U8X8_MSG_DELAY_100NANO:
        __NOP();
        break;

    case U8X8_MSG_DELAY_NANO:
        break;

    default:
        break;
    }

    return 1;
}

void u8g2_Printf(OLED_t *self, uint16_t x, uint16_t y, const char *format,
                 ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->PrintfBuffer, format, arg);
    va_end(arg);

    u8g2_DrawUTF8(&self->u8g2, x, y, (char *)self->PrintfBuffer);
}

#endif