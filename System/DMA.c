#include <stdlib.h>

#include "DMA.h"

void DMA_Init_(DMA_t *self) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAx(self->DMAx), ENABLE);

    DMA_InitTypeDef DMA_InitStruct = {
        .DMA_PeripheralBaseAddr = self->sourceAddr,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize(self->DataSize),
        .DMA_PeripheralInc      = self->sourceInc ? DMA_PeripheralInc_Enable
                                                  : DMA_PeripheralInc_Disable,

        .DMA_MemoryBaseAddr = self->targetAddr,
        .DMA_MemoryDataSize = DMA_MemoryDataSize(self->DataSize),
        .DMA_MemoryInc =
            self->targetInc ? DMA_MemoryInc_Enable : DMA_MemoryInc_Disable,

        .DMA_DIR        = DMA_DIR_PeripheralSRC,
        .DMA_BufferSize = self->BufferSize,
        .DMA_Priority   = DMA_Priority_Medium,
        .DMA_M2M        = self->M2M ? DMA_M2M_Enable : DMA_M2M_Disable,
        .DMA_Mode       = self->Circular ? DMA_Mode_Circular : DMA_Mode_Normal,
    };

    self->DMAy_Channelx = DMAy_Channelx(self->DMAx, self->channel);

    DMA_Init(self->DMAy_Channelx, &DMA_InitStruct);
}

void DMA_Cmd_(DMA_t *self) { DMA_Cmd(self->DMAy_Channelx, ENABLE); }