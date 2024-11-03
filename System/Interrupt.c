#include "Interrupt.h"

void GPIO_Interrupt_Init(GPIOInterrut_t *self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(self->GPIO_PortSource, self->GPIO_PinSource);

    EXTI_InitTypeDef EXTI_InitStruct = {
        .EXTI_Line = self->EXTI_Line,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = self->EXTI_Trigger,
        .EXTI_LineCmd = ENABLE,
    };
    EXTI_Init(&EXTI_InitStruct);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}

void TIM_Interrupt_Init(TIMInterrupt_t *self) {
    TIM_ITConfig(self->TIMx, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);

    TIM_ClearFlag(self->TIMx, TIM_FLAG_Update);
    TIM_Cmd(self->TIMx, ENABLE);
}

void USART_Interrupt_Init(USARTInterrupt_t *self) {
    USART_ITConfig(self->USARTx, self->USART_IT, ENABLE);

    NVIC_PriorityGroupConfig(self->NVIC_PriorityGroup);

    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}