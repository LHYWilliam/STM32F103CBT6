#include <stdarg.h>

#include "Interrupt.h"
#include "Serial.h"
#include "USART.h"

Serial_t *DefaultSerial;

void Serial_Init(Serial_t *self) {
    GPIO_t GPIO;

    if (self->TX[0]) {
        GPIO.Mode = GPIO_Mode_AF_PP;
        GPIO_InitPin(&GPIO, self->TX);
    }

    if (self->RX[0]) {
        GPIO.Mode = GPIO_Mode_IPU;
        GPIO_InitPin(&GPIO, self->RX);
    }

    USART_t USART = {
        .USARTx   = self->USARTx,
        .BaudRate = self->BaudRate,
        .Mode     = (self->TX[0] ? USART_Mode_Tx : 0) |
                (self->RX[0] ? USART_Mode_Rx : 0),
    };
    USART_Init_(&USART);

    if (self->Interrupt) {
        USARTInterrupt_t Interrupt = {
            .USARTx                            = self->USARTx,
            .USART_IT                          = self->Interrupt,
            .NVIC_IRQChannel                   = USARTx_IRQn(self->USARTx),
            .NVIC_PriorityGroup                = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = self->Priority,
        };
        USART_Interrupt_Init(&Interrupt);
    }

    if (self->DMA) {
        USART_DMACmd(self->USARTx, self->DMA, ENABLE);
    }

    if (self->Default) {
        DefaultSerial = self;
    }
}

void Serial_SendByte(Serial_t *self, uint8_t Byte) {
    while (USART_GetFlagStatus(self->USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(self->USARTx, Byte);
}

void Serial_SendBytes(Serial_t *self, uint8_t *Bytes, uint16_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        Serial_SendByte(self, Bytes[i]);
    }
}

void Serial_Printf(Serial_t *self, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->PrintfBuffer, format, arg);
    va_end(arg);

    for (uint8_t i = 0; self->PrintfBuffer[i]; i++) {
        Serial_SendByte(self, self->PrintfBuffer[i]);
    }
}

int fputc(int ch, FILE *f) {
    USART_SendData(DefaultSerial->USARTx, (uint8_t)ch);

    while (USART_GetFlagStatus(DefaultSerial->USARTx, USART_FLAG_TC) == RESET) {
    }

    return ch;
}

int GetKey(void) {

    while (!(DefaultSerial->USARTx->SR & USART_FLAG_RXNE))
        ;

    return ((int)(DefaultSerial->USARTx->DR & 0x1FF));
}