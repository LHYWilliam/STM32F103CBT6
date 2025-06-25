#include "Interrupt.h"

void GPIO_Interrupt_Init(GPIOInterrut_t *Self) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(Self->GPIO_PortSource, Self->GPIO_PinSource);

    EXTI_InitTypeDef EXTI_InitStruct = {
        .EXTI_Line    = Self->EXTI_Line,
        .EXTI_Mode    = EXTI_Mode_Interrupt,
        .EXTI_Trigger = Self->EXTI_Trigger,
        .EXTI_LineCmd = ENABLE,
    };
    EXTI_Init(&EXTI_InitStruct);

    NVIC_PriorityGroupConfig(Self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = Self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            Self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = Self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd         = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}

void TIM_Interrupt_Init(TIMInterrupt_t *Self) {
    TIM_ITConfig(Self->TIMx, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(Self->NVIC_PriorityGroup);
    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = Self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            Self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = Self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd         = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);

    TIM_ClearFlag(Self->TIMx, TIM_FLAG_Update);
    TIM_Cmd(Self->TIMx, ENABLE);
}

void USART_Interrupt_Init(USARTInterrupt_t *Self) {
    USART_ITConfig(Self->USARTx, Self->USART_IT, ENABLE);

    NVIC_PriorityGroupConfig(Self->NVIC_PriorityGroup);

    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = Self->NVIC_IRQChannel,
        .NVIC_IRQChannelPreemptionPriority =
            Self->NVIC_IRQChannelPreemptionPriority,
        .NVIC_IRQChannelSubPriority = Self->NVIC_IRQChannelSubPriority,
        .NVIC_IRQChannelCmd         = ENABLE,
    };
    NVIC_Init(&NVIC_InitStruct);
}