#include "ICM42688_SPI.h"
#include "GPIO.h"

void ICM42688_HWSPI_Init(ICM42688_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode      = GPIO_Mode_AF_PP;
    Self->SCLK_ODR = GPIO_InitPin(&GPIO, Self->SCLK);
    GPIO_InitPin(&GPIO, Self->MISO);
    Self->MOSI_ODR = GPIO_InitPin(&GPIO, Self->MOSI);

    GPIO.Mode    = GPIO_Mode_Out_PP;
    Self->CS_ODR = GPIO_InitPin(&GPIO, Self->CS);

    GPIO_Write(Self->SCLK_ODR, 1);
    GPIO_Write(Self->MOSI_ODR, 1);
    GPIO_Write(Self->CS_ODR, 1);

    SPI_RCC_APBxPeriphClockCmd(Self->SPIx)(RCC_APBxPeriph_SPIy(Self->SPIx),
                                           ENABLE);

    SPI_InitTypeDef SPI = {
        .SPI_Direction         = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode              = SPI_Mode_Master,
        .SPI_DataSize          = SPI_DataSize_8b,
        .SPI_CPOL              = SPI_CPOL_High,
        .SPI_CPHA              = SPI_CPHA_2Edge,
        .SPI_NSS               = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256,
        .SPI_FirstBit          = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial     = 7,
    };
    SPI_Init(Self->SPIx, &SPI);

    SPI_Cmd(Self->SPIx, ENABLE);
}

#define ICM42688_HWSPI_Start(Self) GPIO_Write(Self->CS_ODR, 0)

#define ICM42688_HWSPI_Stop(Self)  GPIO_Write(Self->CS_ODR, 1)

uint8_t ICM42688_HWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t Byte) {
    uint8_t retry = 0;
    while (SPI_I2S_GetFlagStatus(Self->SPIx, SPI_I2S_FLAG_TXE) == RESET) {
        retry++;
        if (retry > 200) {
            return 0;
        }
    }
    SPI_I2S_SendData(Self->SPIx, Byte);

    retry = 0;
    while (SPI_I2S_GetFlagStatus(Self->SPIx, SPI_I2S_FLAG_RXNE) == RESET) {
        retry++;
        if (retry > 200) {
            return 0;
        }
    }

    return SPI_I2S_ReceiveData(Self->SPIx);
}

void ICM42688_HWSPI_ReadWriteBytes(ICM42688_t *Self, uint8_t *Datas,
                                   uint8_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        *Datas = ICM42688_HWSPI_ReadWriteByte(Self, *Datas);
        Datas++;
    }
}

uint8_t ICM42688_HWSPI_ReadReg(ICM42688_t *Self, uint8_t Reg) {
    Reg |= 0x80;

    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    uint8_t Value = ICM42688_HWSPI_ReadWriteByte(Self, 0xFF);
    ICM42688_HWSPI_Stop(Self);

    return Value;
}

void ICM42688_HWSPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Datas,
                             uint16_t Length) {
    Reg |= 0x80;

    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    while (Length) {
        *Datas = ICM42688_HWSPI_ReadWriteByte(Self, 0x00);
        Length--;
        Datas++;
    }
    ICM42688_HWSPI_Stop(Self);
}

uint8_t ICM42688_HWSPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Value) {
    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    ICM42688_HWSPI_ReadWriteByte(Self, Value);
    ICM42688_HWSPI_Stop(Self);

    return 0;
}
