#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

int main() {
    OLED_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Pin = GPIO_Pin_2,
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);

    OLED_ShowString(1, 1, "Hello World");

    for (;;) {
    }
}