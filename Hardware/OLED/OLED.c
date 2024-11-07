#include "OLED.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Delay.h"
#include "GPIO.h"

#if U8G2

uint32_t SCL_ODR;
uint32_t SDA_ODR;

uint8_t u8g2_gpio_and_delay_sw_i2c(U8X8_UNUSED u8x8_t *u8x8,
                                   U8X8_UNUSED uint8_t msg,
                                   U8X8_UNUSED uint8_t arg_int,
                                   U8X8_UNUSED void *arg_ptr);

#else

#include "OLED_Font.h"
#include "OLED_I2C.h"

#endif

void OLED_Init(OLED_t *self) {
    Delay_ms(100);

    GPIO_t SCL = {
        .Mode = GPIO_Mode_Out_OD,
    };
    strcpy(SCL.GPIOxPiny, self->SCL);
    GPIO_Init_(&SCL);

    GPIO_t SDA = {
        .Mode = GPIO_Mode_Out_OD,
    };
    strcpy(SDA.GPIOxPiny, self->SDA);
    GPIO_Init_(&SDA);

#if U8G2

    SCL_ODR = GPIO_ODR(self->SCL);
    SDA_ODR = GPIO_ODR(self->SDA);

#else

    self->SCL_ODR = GPIO_ODR(self->SCL);
    self->SDA_ODR = GPIO_ODR(self->SDA);

#endif

#if U8G2

    if (self->I2C) {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(
            &self->u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_sw_i2c);
    }

    u8g2_InitDisplay(&self->u8g2);
    u8g2_SetPowerSave(&self->u8g2, 0);

#else

    OLED_WriteCommand(self, 0xAE); // 关闭显示

    OLED_WriteCommand(self, 0xD5); // 设置时钟
    OLED_WriteCommand(self, 0x80);

    OLED_WriteCommand(self, 0xA8); // 设置多路复
    OLED_WriteCommand(self, 0x3F);

    OLED_WriteCommand(self, 0xD3); // 设置显示偏移
    OLED_WriteCommand(self, 0x00);

    OLED_WriteCommand(self, 0x40); // 设置起始行

    OLED_WriteCommand(self, 0xA1); // 设置左右方向

    OLED_WriteCommand(self, 0xC8); // 设置上下方向

    OLED_WriteCommand(self, 0xDA); // 设置COM硬件引脚配置
    OLED_WriteCommand(self, 0x12);

    OLED_WriteCommand(self, 0x81); // 调节亮度
    OLED_WriteCommand(self, 0xCF);

    OLED_WriteCommand(self, 0xD9); // 设置预充电周期
    OLED_WriteCommand(self, 0xF1);

    OLED_WriteCommand(self, 0xDB); // 设置VCOMH
    OLED_WriteCommand(self, 0x30);

    OLED_WriteCommand(self, 0xA4); // 全局显示开启

    OLED_WriteCommand(self, 0xA6); // 设置显示方式

    OLED_WriteCommand(self, 0x8D); // 设置电荷泵
    OLED_WriteCommand(self, 0x14);

    OLED_WriteCommand(self, 0xAF); // 打开显示

    OLED_Clear(self);

#endif
}

#if !U8G2

void OLED_SetCursor(OLED_t *self, uint8_t width, uint8_t height) {
    OLED_WriteCommand(self, 0xB0 | width);
    OLED_WriteCommand(self, 0x10 | ((height & 0xF0) >> 4));
    OLED_WriteCommand(self, 0x00 | (height & 0x0F));
}

void OLED_Clear(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(self, j, 0);
        for (uint8_t i = 0; i < 128; i++) {
            OLED_WriteData(self, 0x00);
        }
    }
}

void OLED_ShowChar(OLED_t *self, uint8_t Line, uint8_t Column, char Char) {
    OLED_SetCursor(self, (Line - 1) * 2, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteData(self, OLED_F8x16[Char - ' '][i]);
    }

    OLED_SetCursor(self, (Line - 1) * 2 + 1, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteData(self, OLED_F8x16[Char - ' '][i + 8]);
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

#else

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
        return 0;
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