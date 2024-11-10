#include "GPIO.h"

void GPIO_Init_(GPIO_t *self) {
    GPIO_InitTypeDef GPIO_InitStruct = {
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = self->Mode,
    };

    char *temp = self->GPIOxPiny;
    do {
        self->GPIOx = GPIOx(temp);
        self->GPIO_Pin = GPIO_Pinx(temp);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx(temp), ENABLE);
        GPIO_InitStruct.GPIO_Pin = self->GPIO_Pin,
        GPIO_Init(self->GPIOx, &GPIO_InitStruct);

    } while ((temp = strchr(temp, '|'), temp) && (temp = temp + 2));
}

void GPIO_InitPin(GPIO_t *GPIO, const char *Pin) {
    strcpy(GPIO->GPIOxPiny, Pin);
    GPIO_Init_(GPIO);
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