#include "GPIO.h"

uint32_t GPIO_InitPin(GPIO_t *self, const GPIOxPiny_t Pin) {
    strcpy(self->GPIOxPiny, Pin);

    GPIO_InitTypeDef GPIO_InitStruct = {
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = self->Mode,
    };

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx(self->GPIOxPiny), ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pinx(self->GPIOxPiny),
    GPIO_Init(GPIOx(self->GPIOxPiny), &GPIO_InitStruct);

    if (self->Mode == GPIO_Mode_AIN || self->Mode == GPIO_Mode_IN_FLOATING ||
        self->Mode == GPIO_Mode_IPD || self->Mode == GPIO_Mode_IPU) {
        return GPIO_IDR(self->GPIOxPiny);

    } else {
        return GPIO_ODR(self->GPIOxPiny);
    }
}

uint32_t GPIO_CR(const GPIOxPiny_t Pin) {
    return (Pin[0] == 'A'   ? ((Pin[2] || Pin[1] >= 8) ? GPIOA_CRH : GPIOA_CRL)
            : Pin[0] == 'B' ? ((Pin[2] || Pin[1] >= 8) ? GPIOB_CRH : GPIOB_CRL)
            : Pin[0] == 'C' ? ((Pin[2] || Pin[1] >= 8) ? GPIOC_CRH : GPIOC_CRL)
                            : NULL);
}

uint32_t GPIO_IDR(const GPIOxPiny_t Pin) {
    return (Pin[0] == 'A'   ? (Pin[2] ? BITBAND(GPIOA_IDR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOA_IDR, Pin[1] - '0'))
            : Pin[0] == 'B' ? (Pin[2] ? BITBAND(GPIOB_IDR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOB_IDR, Pin[1] - '0'))
            : Pin[0] == 'C' ? (Pin[2] ? BITBAND(GPIOC_IDR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOC_IDR, Pin[1] - '0'))
                            : NULL);
}

uint32_t GPIO_ODR(const GPIOxPiny_t Pin) {
    return (Pin[0] == 'A'   ? (Pin[2] ? BITBAND(GPIOA_ODR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOA_ODR, Pin[1] - '0'))
            : Pin[0] == 'B' ? (Pin[2] ? BITBAND(GPIOB_ODR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOB_ODR, Pin[1] - '0'))
            : Pin[0] == 'C' ? (Pin[2] ? BITBAND(GPIOC_ODR, 10 + Pin[2] - '0')
                                      : BITBAND(GPIOC_ODR, Pin[1] - '0'))
                            : NULL);
}