#include <stdlib.h>

#include "DMA.h"

void DMA_Init_(DMA_t *Self) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAx(Self->DMAx), ENABLE);

    DMA_InitTypeDef DMA_InitStruct = {
        .DMA_PeripheralBaseAddr = Self->sourceAddr,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize(Self->DataSize),
        .DMA_PeripheralInc      = Self->sourceInc ? DMA_PeripheralInc_Enable
                                                  : DMA_PeripheralInc_Disable,

        .DMA_MemoryBaseAddr = Self->targetAddr,
        .DMA_MemoryDataSize = DMA_MemoryDataSize(Self->DataSize),
        .DMA_MemoryInc =
            Self->targetInc ? DMA_MemoryInc_Enable : DMA_MemoryInc_Disable,

        .DMA_DIR        = DMA_DIR_PeripheralSRC,
        .DMA_BufferSize = Self->BufferSize,
        .DMA_Priority   = DMA_Priority_Medium,
        .DMA_M2M        = Self->M2M ? DMA_M2M_Enable : DMA_M2M_Disable,
        .DMA_Mode       = Self->Circular ? DMA_Mode_Circular : DMA_Mode_Normal,
    };

    Self->DMAy_Channelx = DMAy_Channelx(Self->DMAx, Self->channel);

    DMA_Init(Self->DMAy_Channelx, &DMA_InitStruct);
}

void DMA_Cmd_(DMA_t *Self) { DMA_Cmd(Self->DMAy_Channelx, ENABLE); }