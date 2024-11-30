#include "W25Q64.h"
#include "GPIO.h"

void W25Q64_Init(W25Q64_t *self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_Mode_Out_PP;
    GPIO_InitPin(&GPIO, self->MOSI);
    GPIO_InitPin(&GPIO, self->SCK);
    GPIO_InitPin(&GPIO, self->CS);

    GPIO.Mode = GPIO_Mode_IPU;
    GPIO_InitPin(&GPIO, self->MISO);

    self->MOSI_ODR = GPIO_ODR(self->MOSI);
    self->MISO_IDR = GPIO_IDR(self->MISO);
    self->SCK_ODR = GPIO_ODR(self->SCK);
    self->CS_ODR = GPIO_ODR(self->CS);

    GPIO_Write(self->SCK_ODR, 0);
    GPIO_Write(self->CS_ODR, 1);
}

#define W25Q64_Start(self) GPIO_Write(self->CS_ODR, 0)
#define W25Q64_Stop(self)  GPIO_Write(self->CS_ODR, 1)

uint8_t W25Q64_SwapByte(W25Q64_t *self, uint8_t Send) {
    uint8_t Receive = 0x00;

    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(self->MOSI_ODR, Send & (0x80 >> i));

        GPIO_Write(self->SCK_ODR, 1);

        if (GPIO_ReadInput(self->MISO_IDR)) {
            Receive |= (0x80 >> i);
        }

        GPIO_Write(self->SCK_ODR, 0);
    }

    return Receive;
}

void W25Q64_ReadID(W25Q64_t *self, uint8_t *MID, uint16_t *DID) {
    W25Q64_Start(self);

    W25Q64_SwapByte(self, W25Q64_JedecIDCommand);

    *MID = W25Q64_SwapByte(self, W25Q64_DummyByte);
    *DID = W25Q64_SwapByte(self, W25Q64_DummyByte);
    *DID <<= 8;
    *DID |= W25Q64_SwapByte(self, W25Q64_DummyByte);

    W25Q64_Stop(self);
}

void W25Q64_WriteEnable(W25Q64_t *self) {
    W25Q64_Start(self);
    W25Q64_SwapByte(self, W25Q64_WriteEnableCommand);
    W25Q64_Stop(self);
}

void W25Q64_WaitBusy(W25Q64_t *self) {
    W25Q64_Start(self);

    W25Q64_SwapByte(self, W25Q64_ReadStatusCommand);
    while ((W25Q64_SwapByte(self, W25Q64_DummyByte) & 0x01) == 0x01)
        ;

    W25Q64_Stop(self);
}

void W25Q64_ReadData(W25Q64_t *self, uint32_t Address, uint8_t *DataArray,
                     uint32_t Count) {
    W25Q64_Start(self);

    W25Q64_SwapByte(self, W25Q64_ReadDataCommand);
    W25Q64_SwapByte(self, Address >> 16);
    W25Q64_SwapByte(self, Address >> 8);
    W25Q64_SwapByte(self, Address);

    for (uint32_t i = 0; i < Count; i++) {
        DataArray[i] = W25Q64_SwapByte(self, W25Q64_DummyByte);
    }

    W25Q64_Stop(self);
}

void W25Q64_PageProgram(W25Q64_t *self, uint32_t Address, uint8_t *Data,
                        uint16_t Count) {
    W25Q64_WriteEnable(self);
    W25Q64_Start(self);

    W25Q64_SwapByte(self, W25Q64_PageProgramCommand);
    W25Q64_SwapByte(self, Address >> 16);
    W25Q64_SwapByte(self, Address >> 8);
    W25Q64_SwapByte(self, Address);
    for (uint16_t i = 0; i < Count; i++) {
        W25Q64_SwapByte(self, Data[i]);
    }

    W25Q64_Stop(self);
    W25Q64_WaitBusy(self);
}

void W25Q64_SectorErase(W25Q64_t *self, uint32_t Address) {
    W25Q64_WriteEnable(self);
    W25Q64_Start(self);

    W25Q64_SwapByte(self, W25Q64_SectorErase4KBCommand);
    W25Q64_SwapByte(self, Address >> 16);
    W25Q64_SwapByte(self, Address >> 8);
    W25Q64_SwapByte(self, Address);

    W25Q64_Stop(self);
    W25Q64_WaitBusy(self);
}