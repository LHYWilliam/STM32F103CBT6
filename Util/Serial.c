#include <stdarg.h>

#include "GPIO.h"
#include "Interrupt.h"
#include "Serial.h"
#include "Usart.h"

void Serial_Init(Serial_t *self) {
    if (self->TX[0]) {
        GPIO_t GPIO = {
            .Mode = GPIO_Mode_AF_PP,
        };
        GPIO_InitPin(GPIO, self->TX);
    }

    if (self->RX[0]) {
        GPIO_t GPIO = {
            .Mode = GPIO_Mode_IPU,
        };
        GPIO_InitPin(GPIO, self->RX);
    }

    USART_t usart = {
        .RCC_APBPeriph = RCC_APBPeriphx_USARTx(self->USARTx),
        .USARTx = self->USARTx,
        .USART_Mode = (self->TX[0] ? USART_Mode_Tx : 0) |
                      (self->RX[0] ? USART_Mode_Rx : 0),
    };
    USART_Init_(&usart);

    if (self->Interrupt) {
        USARTInterrupt_t interrupt = {
            .USARTx = self->USARTx,
            .USART_IT = USART_IT_RXNE,
            .NVIC_IRQChannel = USARTx_IRQn(self->USARTx),
            .NVIC_PriorityGroup = NVIC_PriorityGroup_4,
            .NVIC_IRQChannelPreemptionPriority = 2,
        };
        USART_Interrupt_Init(&interrupt);
    }

    if (self->DMA) {
        USART_DMACmd(self->USARTx, USART_DMAReq_Rx, ENABLE);
    }

    self->count = 0;
    self->RecieveFlag = RESET;
    self->type = None;
}

void Serial_SendByte(Serial_t *self, uint8_t byte) {
    while (USART_GetFlagStatus(self->USARTx, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(self->USARTx, byte);
}

void Serial_SendHex(Serial_t *self, uint8_t byte) {
    Serial_SendByte(self, byte);
}

void Serial_SendString(Serial_t *self, char *format, ...) {
    char string[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(string, format, arg);
    va_end(arg);
    for (uint8_t i = 0; string[i] != '\0'; i++) {
        Serial_SendByte(self, string[i]);
    }
}

void Serial_SendHexPack(Serial_t *self, uint8_t *array, uint16_t length) {
    Serial_SendByte(self, 0xFF);
    for (uint8_t i = 0; i < length; i++) {
        Serial_SendByte(self, array[i]);
    }
    Serial_SendByte(self, 0xFE);
}

void Serial_SendStringPack(Serial_t *self, char *string) {
    Serial_SendString(self, ">%s\r\n", string);
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
            self->HexData[self->count++] = self->ByteData;
        }
        break;

    case StringPack:
        // if (serial->count >= 1 &&
        //     serial->StringData[serial->count - 1] == '\r' &&
        //     serial->ByteData == '\n') {
        //     serial->StringData[serial->count - 1] = '\0';
        if (self->count >= 1 && self->ByteData == '\r') {
            self->StringData[self->count] = '\0';
            self->RecieveFlag = SET;
        } else if (self->ByteData == 0x08) {
            self->count--;
        } else {
            self->StringData[self->count++] = self->ByteData;
        }
        break;

    default:
        break;
    }
}

void Serial_Clear(Serial_t *self) {
    self->count = 0;
    self->type = None;
    self->RecieveFlag = RESET;
}