#ifndef SERIAL_H
#define SERIAL_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

#define USARTx_IRQn(x)             \
    ((x) == USART1   ? USART1_IRQn \
     : (x) == USART2 ? USART2_IRQn \
     : (x) == USART3 ? USART3_IRQn \
                     : NULL)

typedef enum {
    None,
    Byte,
    HexPack,
    StringPack,
} PackType;

typedef struct {
    GPIOxPiny_t TX;
    GPIOxPiny_t RX;

    USART_TypeDef *USARTx;
    uint32_t       BaudRate;

    uint8_t Priority;
    uint8_t Interrupt;

    uint8_t DMA;

    uint8_t PrintfBuffer[128];

    uint8_t Default;
} Serial_t;

void Serial_Init(Serial_t *Self);
void Serial_SendByte(Serial_t *Self, uint8_t Byte);
void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint16_t Length);
void Serial_Printf(Serial_t *Self, const char *format, ...);

#endif