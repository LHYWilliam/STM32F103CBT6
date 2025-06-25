#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    uint8_t GPIO_PortSource;
    uint8_t GPIO_PinSource;

    uint32_t            EXTI_Line;
    EXTITrigger_TypeDef EXTI_Trigger;

    uint8_t  NVIC_IRQChannel;
    uint32_t NVIC_PriorityGroup;
    uint8_t  NVIC_IRQChannelPreemptionPriority;
    uint8_t  NVIC_IRQChannelSubPriority;
} GPIOInterrut_t;

typedef struct {
    TIM_TypeDef *TIMx;

    uint8_t  NVIC_IRQChannel;
    uint32_t NVIC_PriorityGroup;
    uint8_t  NVIC_IRQChannelPreemptionPriority;
    uint8_t  NVIC_IRQChannelSubPriority;
} TIMInterrupt_t;

typedef struct {
    USART_TypeDef *USARTx;

    uint16_t USART_IT;

    uint8_t  NVIC_IRQChannel;
    uint32_t NVIC_PriorityGroup;
    uint8_t  NVIC_IRQChannelPreemptionPriority;
    uint8_t  NVIC_IRQChannelSubPriority;

} USARTInterrupt_t;

void GPIO_Interrupt_Init(GPIOInterrut_t *Self);
void TIM_Interrupt_Init(TIMInterrupt_t *Self);
void USART_Interrupt_Init(USARTInterrupt_t *Self);

#endif