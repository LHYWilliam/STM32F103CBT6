#include "W25Q64.h"

void W25Q64_Init(W25Q64_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode      = GPIO_Mode_Out_PP;
    Self->MOSI_ODR = GPIO_InitPin(&GPIO, Self->MOSI);
    Self->SCK_ODR  = GPIO_InitPin(&GPIO, Self->SCK);
    Self->CS_ODR   = GPIO_InitPin(&GPIO, Self->CS);

    GPIO.Mode      = GPIO_Mode_IPU;
    Self->MISO_IDR = GPIO_InitPin(&GPIO, Self->MISO);

    GPIO_Write(Self->SCK_ODR, 0);
    GPIO_Write(Self->CS_ODR, 1);
}

#define W25Q64_Start(Self) GPIO_Write(Self->CS_ODR, 0)
#define W25Q64_Stop(Self)  GPIO_Write(Self->CS_ODR, 1)

uint8_t W25Q64_SwapByte(W25Q64_t *Self, uint8_t Send) {
    uint8_t Receive = 0x00;

    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(Self->MOSI_ODR, Send & (0x80 >> i));

        GPIO_Write(Self->SCK_ODR, 1);

        if (GPIO_ReadInput(Self->MISO_IDR)) {
            Receive |= (0x80 >> i);
        }

        GPIO_Write(Self->SCK_ODR, 0);
    }

    return Receive;
}

void W25Q64_ReadID(W25Q64_t *Self, uint8_t *MID, uint16_t *DID) {
    W25Q64_Start(Self);

    W25Q64_SwapByte(Self, W25Q64_JedecIDCommand);

    *MID = W25Q64_SwapByte(Self, W25Q64_DummyByte);
    *DID = W25Q64_SwapByte(Self, W25Q64_DummyByte);
    *DID <<= 8;
    *DID |= W25Q64_SwapByte(Self, W25Q64_DummyByte);

    W25Q64_Stop(Self);
}

void W25Q64_WriteEnable(W25Q64_t *Self) {
    W25Q64_Start(Self);
    W25Q64_SwapByte(Self, W25Q64_WriteEnableCommand);
    W25Q64_Stop(Self);
}

void W25Q64_WaitBusy(W25Q64_t *Self) {
    W25Q64_Start(Self);

    W25Q64_SwapByte(Self, W25Q64_ReadStatusCommand);
    while ((W25Q64_SwapByte(Self, W25Q64_DummyByte) & 0x01) == 0x01)
        ;

    W25Q64_Stop(Self);
}

void W25Q64_ReadData(W25Q64_t *Self, uint32_t Address, uint8_t *DataArray,
                     uint32_t Count) {
    W25Q64_Start(Self);

    W25Q64_SwapByte(Self, W25Q64_ReadDataCommand);
    W25Q64_SwapByte(Self, Address >> 16);
    W25Q64_SwapByte(Self, Address >> 8);
    W25Q64_SwapByte(Self, Address);

    for (uint32_t i = 0; i < Count; i++) {
        DataArray[i] = W25Q64_SwapByte(Self, W25Q64_DummyByte);
    }

    W25Q64_Stop(Self);
}

void W25Q64_PageProgram(W25Q64_t *Self, uint32_t Address, uint8_t *Data,
                        uint16_t Count) {
    W25Q64_WriteEnable(Self);
    W25Q64_Start(Self);

    W25Q64_SwapByte(Self, W25Q64_PageProgramCommand);
    W25Q64_SwapByte(Self, Address >> 16);
    W25Q64_SwapByte(Self, Address >> 8);
    W25Q64_SwapByte(Self, Address);
    for (uint16_t i = 0; i < Count; i++) {
        W25Q64_SwapByte(Self, Data[i]);
    }

    W25Q64_Stop(Self);
    W25Q64_WaitBusy(Self);
}

void W25Q64_SectorErase(W25Q64_t *Self, uint32_t Address) {
    W25Q64_WriteEnable(Self);
    W25Q64_Start(Self);

    W25Q64_SwapByte(Self, W25Q64_SectorErase4KBCommand);
    W25Q64_SwapByte(Self, Address >> 16);
    W25Q64_SwapByte(Self, Address >> 8);
    W25Q64_SwapByte(Self, Address);

    W25Q64_Stop(Self);
    W25Q64_WaitBusy(Self);
}