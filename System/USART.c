#include <stdlib.h>

#include "USART.h"

void USART_Init_(USART_t *Self) {
    RCC_APBxPeriphClockCmd(Self->USARTx)(RCC_APBPeriphx_USARTx(Self->USARTx),
                                         ENABLE);

    USART_InitTypeDef USART_InitStruct = {
        .USART_Mode     = Self->Mode,
        .USART_BaudRate = Self->BaudRate,
    };
    USART_Init(Self->USARTx, &USART_InitStruct);

    USART_Cmd(Self->USARTx, ENABLE);
}