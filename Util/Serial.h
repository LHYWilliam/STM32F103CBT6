#ifndef SERIAL_H
#define SERIAL_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define USARTx_IRQn(x)                                                         \
    ((x) == USART1   ? USART1_IRQn                                             \
     : (x) == USART2 ? USART2_IRQn                                             \
     : (x) == USART3 ? USART3_IRQn                                             \
                     : NULL)

typedef enum {
    None,
    Byte,
    HexPack,
    StringPack,
} PackType;

typedef struct {
    char TX[4];
    char RX[4];

    USART_TypeDef *USARTx;
    uint32_t BaudRate;

    uint8_t Priority;
    uint8_t Interrupt;

    uint8_t DMA;

    uint8_t PrintfBuffer[128];
} Serial_t;

void Serial_Init(Serial_t *self);
void Serial_SendByte(Serial_t *self, uint8_t Byte);
void Serial_SendBytes(Serial_t *self, uint8_t *Bytes, uint16_t Length);
void Serial_Printf(Serial_t *self, const char *format, ...);

#endif