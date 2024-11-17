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

void OLED_SetCursor(OLED_t *self, uint8_t width, uint8_t height) {
    static uint8_t OLED_SetCursorCommands[3];
    OLED_SetCursorCommands[0] = 0xB0 | width;
    OLED_SetCursorCommands[1] = 0x10 | ((height & 0xF0) >> 4);
    OLED_SetCursorCommands[2] = 0x00 | (height & 0x0F);
    self->OLED_WriteCommands(self, OLED_SetCursorCommands, 3);
}

void OLED_Clear(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(self, j, 0);
        self->OLED_WriteData(self, 0x00, 128);
    }
}

void OLED_ShowChar(OLED_t *self, uint8_t Line, uint8_t Column, char Char) {
    OLED_SetCursor(self, (Line - 1) * 2, (Column - 1) * 8);
    self->OLED_WriteDatas(self, (uint8_t *)&OLED_F8x16[Char - ' '][0], 8);

    OLED_SetCursor(self, (Line - 1) * 2 + 1, (Column - 1) * 8);
    self->OLED_WriteDatas(self, (uint8_t *)&OLED_F8x16[Char - ' '][8], 8);
}

void OLED_ShowString(OLED_t *self, uint8_t Line, uint8_t Column,
                     const char *String) {
    for (uint8_t i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar(self, Line, Column + i, String[i]);
    }
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

void OLED_ShowNum(OLED_t *self, uint8_t Line, uint8_t Column, uint32_t Number,
                  uint8_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        OLED_ShowChar(self, Line, Column + i,
                      Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_ShowSignedNum(OLED_t *self, uint8_t Line, uint8_t Column,
                        int32_t Number, uint8_t Length) {
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(self, Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar(self, Line, Column, '-');
        Number1 = -Number;
    }
    for (uint8_t i = 0; i < Length; i++) {
        OLED_ShowChar(self, Line, Column + i + 1,
                      Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_ShowHexNum(OLED_t *self, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        uint8_t SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar(self, Line, Column + i, SingleNumber + '0');
        } else {
            OLED_ShowChar(self, Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

void OLED_ShowBinNum(OLED_t *self, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        OLED_ShowChar(self, Line, Column + i,
                      Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

void OLED_Printf(OLED_t *self, uint16_t x, uint16_t y, const char *format,
                 ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->Buffer, format, arg);
    va_end(arg);

    OLED_ShowString(self, x, y, (char *)self->Buffer);
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

void u8g2_Printf(OLED_t *self, u8g2_uint_t x, u8g2_uint_t y, const char *format,
                 ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->Buffer, format, arg);
    va_end(arg);

    u8g2_DrawUTF8(&self->u8g2, x, y, (char *)self->Buffer);
}

#endif