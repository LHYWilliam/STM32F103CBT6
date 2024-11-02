#include "Interrupt.h"

void GPIO_Interrupt_Init(GPIOInterrut_t *self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(self->GPIO_PortSource, self->GPIO_PinSource);

    EXTI_InitTypeDef EXTI_InitStruct = {
        self->EXTI_Line,
        EXTI_Mode_Interrupt,
        self->EXTI_Trigger,
        ENABLE,
    };
    EXTI_Init(&EXTI_InitStruct);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        self->NVIC_IRQChannel,
        self->NVIC_IRQChannelPreemptionPriority,
        self->NVIC_IRQChannelSubPriority,
        ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}

void TIM_Interrupt_Init(TIMInterrupt_t *self) {
    TIM_ITConfig(self->TIMx, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        self->NVIC_IRQChannel,
        self->NVIC_IRQChannelPreemptionPriority,
        self->NVIC_IRQChannelSubPriority,
        ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(self->TIMx, ENABLE);
}

void USART_Interrupt_Init(USARTInterrupt_t *self) {
    USART_ITConfig(self->USARTx, self->USART_IT, ENABLE);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);

    NVIC_InitTypeDef NVIC_InitStruct = {
        self->NVIC_IRQChannel,
        self->NVIC_IRQChannelPreemptionPriority,
        self->NVIC_IRQChannelSubPriority,
        ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}