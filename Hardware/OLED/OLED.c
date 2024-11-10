#include <stdarg.h>

#include "Delay.h"
#include "GPIO.h"
#include "OLED.h"

#if U8G2

uint32_t SCL_ODR;
uint32_t SDA_ODR;
uint8_t u8g2_gpio_and_delay_sw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
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
#if U8G2
    if (self->U8g2) {
        if (self->I2C) {
            GPIO_t GPIO;
            GPIO.Mode = GPIO_Mode_Out_OD;
            GPIO_InitPin(&GPIO, self->SCL);
            GPIO_InitPin(&GPIO, self->SDA);

            SCL_ODR = GPIO_ODR(self->SCL);
            SDA_ODR = GPIO_ODR(self->SDA);

            GPIO_Write(SCL_ODR, 1);
            GPIO_Write(SDA_ODR, 1);

        } else if (self->SPI) {
            GPIO_t GPIO;
            GPIO.Mode = self->SPIx ? GPIO_Mode_AF_PP : GPIO_Mode_Out_PP;
            GPIO_InitPin(&GPIO, self->D0);
            GPIO_InitPin(&GPIO, self->D1);
            GPIO.Mode = GPIO_Mode_Out_PP;
            GPIO_InitPin(&GPIO, self->RES);
            GPIO_InitPin(&GPIO, self->DC);
            GPIO_InitPin(&GPIO, self->CS);

            D0_ODR = GPIO_ODR(self->D0);
            D1_ODR = GPIO_ODR(self->D1);
            RES_ODR = GPIO_ODR(self->RES);
            DC_ODR = GPIO_ODR(self->DC);
            CS_ODR = GPIO_ODR(self->CS);

            GPIO_Write(RES_ODR, 1);
            GPIO_Write(DC_ODR, 1);
            GPIO_Write(CS_ODR, 1);

            if (self->SPIx) {
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,
                                       ENABLE); // 使能SPI1时钟

                SPI_InitTypeDef SPI_InitStructure; // 定义结构体变量
                SPI_InitStructure.SPI_Mode =
                    SPI_Mode_Master; // 模式，选择为SPI主模式
                SPI_InitStructure.SPI_Direction =
                    SPI_Direction_1Line_Tx; // 方向，选择2线全双工
                SPI_InitStructure.SPI_DataSize =
                    SPI_DataSize_8b; // 数据宽度，选择为8位
                SPI_InitStructure.SPI_FirstBit =
                    SPI_FirstBit_MSB; // 先行位，选择高位先行
                SPI_InitStructure.SPI_BaudRatePrescaler =
                    SPI_BaudRatePrescaler_32; // 波特率分频，选择128分频
                SPI_InitStructure.SPI_CPOL =
                    SPI_CPOL_Low; // SPI极性，选择低极性
                SPI_InitStructure.SPI_CPHA =
                    SPI_CPHA_1Edge; // SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
                SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // NSS，选择由软件控制
                SPI_InitStructure.SPI_CRCPolynomial =
                    7; // CRC多项式，暂时用不到，给默认值7
                SPI_Init(
                    self->SPIx,
                    &SPI_InitStructure); // 将结构体变量交给SPI_Init，配置SPI1
                SPIx = self->SPIx;

                /*SPI使能*/
                SPI_Cmd(self->SPIx, ENABLE); // 使能SPI1，开始运行
            }
        }
    } else {

#endif

        if (self->I2C) {
            GPIO_t GPIO;
            GPIO.Mode = GPIO_Mode_Out_OD;
            GPIO_InitPin(&GPIO, self->SCL);
            GPIO_InitPin(&GPIO, self->SDA);

            self->SCL_ODR = GPIO_ODR(self->SCL);
            self->SDA_ODR = GPIO_ODR(self->SDA);

            GPIO_Write(self->SCL_ODR, 1);
            GPIO_Write(self->SDA_ODR, 1);

            self->OLED_WriteData = OLED_I2C_WriteData;
            self->OLED_WriteCommand = OLED_I2C_WriteCommand;

        } else if (self->SPI) {
            GPIO_t GPIO;
            GPIO.Mode = GPIO_Mode_Out_PP;
            GPIO_InitPin(&GPIO, self->D0);
            GPIO_InitPin(&GPIO, self->D1);
            GPIO_InitPin(&GPIO, self->RES);
            GPIO_InitPin(&GPIO, self->DC);
            GPIO_InitPin(&GPIO, self->CS);

            self->D0_ODR = GPIO_ODR(self->D0);
            self->D1_ODR = GPIO_ODR(self->D1);
            self->RES_ODR = GPIO_ODR(self->RES);
            self->DC_ODR = GPIO_ODR(self->DC);
            self->CS_ODR = GPIO_ODR(self->CS);

            GPIO_Write(self->D0, 1);
            GPIO_Write(self->D1, 1);
            GPIO_Write(self->CS_ODR, 1);
            GPIO_Write(self->DC_ODR, 1);
            GPIO_Write(self->RES_ODR, 1);

            self->OLED_WriteData = OLED_SPI_WriteData;
            self->OLED_WriteCommand = OLED_SPI_WriteCommand;
        }

#if U8G2
    }
#endif

#if U8G2
    if (self->U8g2) {
        if (self->I2C) {
            Delay_ms(100);

            u8g2_Setup_ssd1306_i2c_128x64_noname_f(&self->u8g2, U8G2_R0,
                                                   u8x8_byte_sw_i2c,
                                                   u8g2_gpio_and_delay_sw_i2c);
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

        if (self->SPI) {
            GPIO_Write(self->RES_ODR, 0);
        }
        Delay_ms(100);
        if (self->SPI) {
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
    self->OLED_WriteCommand(self, 0xB0 | width);
    self->OLED_WriteCommand(self, 0x10 | ((height & 0xF0) >> 4));
    self->OLED_WriteCommand(self, 0x00 | (height & 0x0F));
}

void OLED_Clear(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(self, j, 0);
        for (uint8_t i = 0; i < 128; i++) {
            self->OLED_WriteData(self, 0x00);
        }
    }
}

void OLED_ShowChar(OLED_t *self, uint8_t Line, uint8_t Column, char Char) {
    OLED_SetCursor(self, (Line - 1) * 2, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++) {
        self->OLED_WriteData(self, OLED_F8x16[Char - ' '][i]);
    }

    OLED_SetCursor(self, (Line - 1) * 2 + 1, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++) {
        self->OLED_WriteData(self, OLED_F8x16[Char - ' '][i + 8]);
    }
}

void OLED_ShowString(OLED_t *self, uint8_t Line, uint8_t Column, char *String) {
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

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                               void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_BYTE_INIT:
        break;

    case U8X8_MSG_BYTE_SET_DC:
        GPIO_Write(DC_ODR, arg_int);
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        GPIO_Write(CS_ODR, 0);
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        GPIO_Write(CS_ODR, 1);
        break;

    case U8X8_MSG_BYTE_SEND:
        for (uint8_t i = 0; i < arg_int; i++) {
            while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) != SET)
                ;
            SPI_I2S_SendData(SPIx, ((uint8_t *)arg_ptr)[i]);
        }
        break;

    default:
        break;
    }

    return 1;
}

uint8_t u8g2_gpio_and_delay_sw_i2c(U8X8_UNUSED u8x8_t *u8x8,
                                   U8X8_UNUSED uint8_t msg,
                                   U8X8_UNUSED uint8_t arg_int,
                                   U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_DELAY_MILLI:
        Delay_ms(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        Delay_us(10);
        break;

    case U8X8_MSG_DELAY_100NANO:
        __NOP();
        break;

    case U8X8_MSG_GPIO_I2C_CLOCK:
        GPIO_Write(SCL_ODR, arg_int);
        break;

    case U8X8_MSG_GPIO_I2C_DATA:
        GPIO_Write(SDA_ODR, arg_int);
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