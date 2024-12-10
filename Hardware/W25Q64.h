#ifndef W25Q64_H
#define W25Q64_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define W25Q64_JedecIDCommand        0x9F
#define W25Q64_ReadDataCommand       0x03
#define W25Q64_ReadStatusCommand     0x05
#define W25Q64_WriteEnableCommand    0x06
#define W25Q64_PageProgramCommand    0x02
#define W25Q64_SectorErase4KBCommand 0x20

#define W25Q64_DummyByte             0xFF

typedef struct {
    char MOSI[4];
    char MISO[4];
    char SCK[4];
    char CS[4];

    uint32_t MOSI_ODR;
    uint32_t MISO_IDR;
    uint32_t SCK_ODR;
    uint32_t CS_ODR;
} W25Q64_t;

void W25Q64_Init(W25Q64_t *self);
void W25Q64_ReadID(W25Q64_t *self, uint8_t *MID, uint16_t *DID);
void W25Q64_ReadData(W25Q64_t *self, uint32_t Address, uint8_t *DataArray,
                     uint32_t Count);
void W25Q64_PageProgram(W25Q64_t *self, uint32_t Address, uint8_t *Data,
                        uint16_t Count);
void W25Q64_SectorErase(W25Q64_t *self, uint32_t Address);

#endif