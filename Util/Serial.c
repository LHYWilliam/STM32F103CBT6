#include <stdarg.h>

#include "GPIO.h"
#include "Interrupt.h"
#include "Serial.h"
#include "Usart.h"

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
        .USARTx = self->USARTx,
        .BaudRate = self->BaudRate,
        .Mode = (self->TX[0] ? USART_Mode_Tx : 0) |
                (self->RX[0] ? USART_Mode_Rx : 0),
    };
    USART_Init_(&USART);

    if (self->Interrupt) {
        USARTInterrupt_t Interrupt = {
            .USARTx = self->USARTx,
            .USART_IT = USART_IT_RXNE,
            .NVIC_IRQChannel = USARTx_IRQn(self->USARTx),
            .NVIC_PriorityGroup = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = self->Priority,
        };
        USART_Interrupt_Init(&Interrupt);
    }

    if (self->DMA) {
        USART_DMACmd(self->USARTx, USART_DMAReq_Rx, ENABLE);
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

void Serial_SendStringPack(Serial_t *self, const char *String) {
    Serial_Printf(self, ">%s\r\n", String);
}

void Serial_Parse(Serial_t *self) {
    self->ByteData = USART_ReceiveData(self->USARTx);

    switch (self->type) {
    case None:

        if (self->ByteData == 0xFF) {
            self->type = HexPack;
        } else if (self->ByteData == '>') {
            self->type = StringPack;
        } else {
            self->type = Byte;
            self->RecieveFlag = SET;
        }
        break;

    case HexPack:
        if (self->ByteData == 0xFE) {
            self->RecieveFlag = SET;
        } else {
            self->HexData[self->Count++] = self->ByteData;
        }
        break;

    case StringPack:
        // if (serial->count >= 1 &&
        //     serial->StringData[serial->count - 1] == '\r' &&
        //     serial->ByteData == '\n') {
        //     serial->StringData[serial->count - 1] = '\0';
        if (self->Count >= 1 && self->ByteData == '\r') {
            self->StringData[self->Count] = '\0';
            self->RecieveFlag = SET;
        } else if (self->ByteData == 0x08) {
            self->Count--;
        } else {
            self->StringData[self->Count++] = self->ByteData;
        }
        break;

    default:
        break;
    }
}

void Serial_Clear(Serial_t *self) {
    self->Count = 0;
    self->type = None;
    self->RecieveFlag = RESET;
}