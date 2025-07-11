#ifndef DMA_H
#define DMA_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define RCC_AHBPeriph_DMAx(x) \
    ((x) == DMA1 ? RCC_AHBPeriph_DMA1 : (x) == DMA2 ? RCC_AHBPeriph_DMA2 : NULL)

#define DMAy_Channelx(x, y)                    \
    ((x) == DMA1 && (y) == 1   ? DMA1_Channel1 \
     : (x) == DMA1 && (y) == 2 ? DMA1_Channel2 \
     : (x) == DMA1 && (y) == 3 ? DMA1_Channel3 \
     : (x) == DMA1 && (y) == 4 ? DMA1_Channel4 \
     : (x) == DMA1 && (y) == 5 ? DMA1_Channel5 \
     : (x) == DMA1 && (y) == 6 ? DMA1_Channel6 \
     : (x) == DMA1 && (y) == 7 ? DMA1_Channel7 \
     : (x) == DMA2 && (y) == 1 ? DMA2_Channel1 \
     : (x) == DMA2 && (y) == 2 ? DMA2_Channel2 \
     : (x) == DMA2 && (y) == 3 ? DMA2_Channel3 \
     : (x) == DMA2 && (y) == 4 ? DMA2_Channel4 \
     : (x) == DMA2 && (y) == 5 ? DMA2_Channel5 \
                               : NULL)

#define DMA_PeripheralDataSize(x)                  \
    ((x) == 8    ? DMA_PeripheralDataSize_Byte     \
     : (x) == 16 ? DMA_PeripheralDataSize_HalfWord \
     : (x) == 32 ? DMA_PeripheralDataSize_Word     \
                 : NULL)

#define DMA_MemoryDataSize(x)                  \
    ((x) == 8    ? DMA_MemoryDataSize_Byte     \
     : (x) == 16 ? DMA_MemoryDataSize_HalfWord \
     : (x) == 32 ? DMA_MemoryDataSize_Word     \
                 : NULL)

typedef struct {
    DMA_TypeDef *DMAx;
    uint8_t      channel;

    uint32_t        sourceAddr;
    FunctionalState sourceInc;

    uint32_t        targetAddr;
    FunctionalState targetInc;

    uint8_t         DataSize;
    uint32_t        BufferSize;
    uint32_t        Circular;
    FunctionalState M2M;

    DMA_Channel_TypeDef *DMAy_Channelx;
} DMA_t;

void DMA_Init_(DMA_t *Self);
void DMA_Cmd_(DMA_t *Self);

#endif