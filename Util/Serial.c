#include <stdarg.h>

#include "Interrupt.h"
#include "Serial.h"
#include "USART.h"

Serial_t *DefaultSerial;

void Serial_Init(Serial_t *Self) {
    GPIO_t GPIO;

    if (Self->TX[0]) {
        GPIO.Mode = GPIO_Mode_AF_PP;
        GPIO_InitPin(&GPIO, Self->TX);
    }

    if (Self->RX[0]) {
        GPIO.Mode = GPIO_Mode_IPU;
        GPIO_InitPin(&GPIO, Self->RX);
    }

    USART_t USART = {
        .USARTx   = Self->USARTx,
        .BaudRate = Self->BaudRate,
        .Mode     = (Self->TX[0] ? USART_Mode_Tx : 0) |
                (Self->RX[0] ? USART_Mode_Rx : 0),
    };
    USART_Init_(&USART);

    if (Self->Interrupt) {
        USARTInterrupt_t Interrupt = {
            .USARTx                            = Self->USARTx,
            .USART_IT                          = Self->Interrupt,
            .NVIC_IRQChannel                   = USARTx_IRQn(Self->USARTx),
            .NVIC_PriorityGroup                = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = Self->Priority,
        };
        USART_Interrupt_Init(&Interrupt);
    }

    if (Self->DMA) {
        USART_DMACmd(Self->USARTx, Self->DMA, ENABLE);
    }

    if (Self->Default) {
        DefaultSerial = Self;
    }
}

void Serial_SendByte(Serial_t *Self, uint8_t Byte) {
    while (USART_GetFlagStatus(Self->USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(Self->USARTx, Byte);
}

void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint16_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        Serial_SendByte(Self, Bytes[i]);
    }
}

void Serial_Printf(Serial_t *Self, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)Self->PrintfBuffer, format, arg);
    va_end(arg);

    for (uint8_t i = 0; Self->PrintfBuffer[i]; i++) {
        Serial_SendByte(Self, Self->PrintfBuffer[i]);
    }
}

int fputc(int ch, FILE *f) {
    USART_SendData(DefaultSerial->USARTx, (uint8_t)ch);

    while (USART_GetFlagStatus(DefaultSerial->USARTx, USART_FLAG_TC) == RESET) {
    }

    return ch;
}
