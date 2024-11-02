#include "USART.h"

void USART_Init_(USART_t *self) {
    if (self->RCC_APBPeriph == RCC_APB2Periph_USART1) {
        RCC_APB2PeriphClockCmd(self->RCC_APBPeriph, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(self->RCC_APBPeriph, ENABLE);
    }

    USART_InitTypeDef USART_InitStruct = {
        9600,
        USART_WordLength_8b,
        USART_StopBits_1,
        USART_Parity_No,
        self->USART_Mode,
        USART_HardwareFlowControl_None,
    };
    USART_Init(self->USARTx, &USART_InitStruct);

    USART_Cmd(self->USARTx, ENABLE);
}