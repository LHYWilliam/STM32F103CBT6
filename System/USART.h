#ifndef USART_H_
#define USART_H_

#include "RTE_Components.h"
#include CMSIS_device_header

#define RCC_APBxPeriphClockCmd(x) \
    ((x) == USART1 ? RCC_APB2PeriphClockCmd : RCC_APB1PeriphClockCmd)

#define RCC_APBPeriphx_USARTx(x)             \
    ((x) == USART1   ? RCC_APB2Periph_USART1 \
     : (x) == USART2 ? RCC_APB1Periph_USART2 \
     : (x) == USART3 ? RCC_APB1Periph_USART3 \
                     : NULL)

typedef struct {
    USART_TypeDef *USARTx;
    uint32_t       Mode;
    uint32_t       BaudRate;
} USART_t;

void USART_Init_(USART_t *self);

#endif