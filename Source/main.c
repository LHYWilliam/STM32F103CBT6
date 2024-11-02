#include "stm32f10x.h"

int main() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Pin = GPIO_Pin_2,
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);

    for (;;) {
    }
}