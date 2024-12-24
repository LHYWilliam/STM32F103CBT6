#include "GPIO.h"

uint32_t GPIO_InitPin(GPIO_t *self, const char *Pin) {
    strcpy(self->GPIOxPiny, Pin);

    GPIO_InitTypeDef GPIO_InitStruct = {
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = self->Mode,
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

uint32_t GPIO_CR(const char *x) {
    return (x[0] == 'A'   ? ((x[2] || x[1] >= 8) ? GPIOA_CRH : GPIOA_CRL)
            : x[0] == 'B' ? ((x[2] || x[1] >= 8) ? GPIOB_CRH : GPIOB_CRL)
            : x[0] == 'C' ? ((x[2] || x[1] >= 8) ? GPIOC_CRH : GPIOC_CRL)
                          : NULL);
}

uint32_t GPIO_IDR(const char *x) {
    return (x[0] == 'A'   ? (x[2] ? BITBAND(GPIOA_IDR, 10 + x[2] - '0')
                                  : BITBAND(GPIOA_IDR, x[1] - '0'))
            : x[0] == 'B' ? (x[2] ? BITBAND(GPIOB_IDR, 10 + x[2] - '0')
                                  : BITBAND(GPIOB_IDR, x[1] - '0'))
            : x[0] == 'C' ? (x[2] ? BITBAND(GPIOC_IDR, 10 + x[2] - '0')
                                  : BITBAND(GPIOC_IDR, x[1] - '0'))
                          : NULL);
}

uint32_t GPIO_ODR(const char *x) {
    return (x[0] == 'A'   ? (x[2] ? BITBAND(GPIOA_ODR, 10 + x[2] - '0')
                                  : BITBAND(GPIOA_ODR, x[1] - '0'))
            : x[0] == 'B' ? (x[2] ? BITBAND(GPIOB_ODR, 10 + x[2] - '0')
                                  : BITBAND(GPIOB_ODR, x[1] - '0'))
            : x[0] == 'C' ? (x[2] ? BITBAND(GPIOC_ODR, 10 + x[2] - '0')
                                  : BITBAND(GPIOC_ODR, x[1] - '0'))
                          : NULL);
}