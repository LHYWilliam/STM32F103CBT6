#include <stdlib.h>

#include "USART.h"

void USART_Init_(USART_t *self) {
    RCC_APBxPeriphClockCmd(self->USARTx)(RCC_APBPeriphx_USARTx(self->USARTx),
                                         ENABLE);

    USART_InitTypeDef USART_InitStruct = {
        .USART_Mode = self->Mode,
        .USART_BaudRate = self->BaudRate,
    };
    USART_Init(self->USARTx, &USART_InitStruct);

    USART_Cmd(self->USARTx, ENABLE);
}