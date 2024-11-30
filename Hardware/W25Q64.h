#ifndef W25Q64_H
#define W25Q64_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define W25Q64_WriteENABLE                 0x06
#define W25Q64_WriteDISABLE                0x04
#define W25Q64_ReadStatusRegister1         0x05
#define W25Q64_ReadStatusRegister2         0x35
#define W25Q64_WriteStatusRegister         0x01
#define W25Q64_PageProgram                 0x02
#define W25Q64_QuadPageProgram             0x32
#define W25Q64_BlockErase64KB              0xD8
#define W25Q64_BlockErase32KB              0x52
#define W25Q64_SectorErase4KB              0x20
#define W25Q64_ChipErase                   0xC7
#define W25Q64_EraseSuspend                0x75
#define W25Q64_EraseResume                 0x7A
#define W25Q64_PowerDown                   0xB9
#define W25Q64_HighPerformanceMode         0xA3
#define W25Q64_ContinuousReadModeReset     0xFF
#define W25Q64_ReleasePowerDownHPMDeviceID 0xAB
#define W25Q64_ManufacturerDeviceID        0x90
#define W25Q64_ReadUniqueID                0x4B
#define W25Q64_JedecID                     0x9F
#define W25Q64_ReadData                    0x03
#define W25Q64_FastRead                    0x0B
#define W25Q64_FastReadDualOutput          0x3B
#define W25Q64_FastReadDualIO              0xBB
#define W25Q64_FastReadQuadOutput          0x6B
#define W25Q64_FastReadQuadIO              0xEB
#define W25Q64_OctalWordReadQuadIO         0xE3

#define W25Q64_DummyByte                   0xFF

typedef enum {
    W25Q64_Command,
    W25Q64_Data,
} W25Q64_ByteType;

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

#endif