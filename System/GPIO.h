#ifndef GPIO_H
#define GPIO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdio.h>
#include <string.h>

#define BITBAND(addr, bitnum)                                        \
    (((addr) & 0xF0000000) + 0x2000000 + (((addr) & 0xFFFFF) << 5) + \
     ((bitnum) << 2))
#define MEM_ADDR(addr)         *((volatile uint32_t *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND((addr), (bitnum)))

#define GPIOA_CRL              (GPIOA_BASE + 0)
#define GPIOB_CRL              (GPIOB_BASE + 0)
#define GPIOC_CRL              (GPIOC_BASE + 0)

#define GPIOA_CRH              (GPIOA_BASE + 4)
#define GPIOB_CRH              (GPIOB_BASE + 4)
#define GPIOC_CRH              (GPIOC_BASE + 4)

#define GPIOA_IDR              (GPIOA_BASE + 8)
#define GPIOB_IDR              (GPIOB_BASE + 8)
#define GPIOC_IDR              (GPIOC_BASE + 8)

#define GPIOA_ODR              (GPIOA_BASE + 12)
#define GPIOB_ODR              (GPIOB_BASE + 12)
#define GPIOC_ODR              (GPIOC_BASE + 12)

#define PAin(n)                BIT_ADDR(GPIOA_IDR, (n))
#define PBin(n)                BIT_ADDR(GPIOB_IDR, (n))
#define PCin(n)                BIT_ADDR(GPIOC_IDR, (n))

#define PAout(n)               BIT_ADDR(GPIOA_ODR, (n))
#define PBout(n)               BIT_ADDR(GPIOB_ODR, (n))
#define PCout(n)               BIT_ADDR(GPIOC_ODR, (n))

#define A0                     "A0"
#define A1                     "A1"
#define A2                     "A2"
#define A3                     "A3"
#define A4                     "A4"
#define A5                     "A5"
#define A6                     "A6"
#define A7                     "A7"
#define A8                     "A8"
#define A9                     "A9"
#define A10                    "A10"
#define A11                    "A11"
#define A12                    "A12"
#define A13                    "A13"
#define A14                    "A14"
#define A15                    "A15"

#define B0                     "B0"
#define B1                     "B1"
#define B2                     "B2"
#define B3                     "B3"
#define B4                     "B4"
#define B5                     "B5"
#define B6                     "B6"
#define B7                     "B7"
#define B8                     "B8"
#define B9                     "B9"
#define B10                    "B10"
#define B11                    "B11"
#define B12                    "B12"
#define B13                    "B13"
#define B14                    "B14"
#define B15                    "B15"

#define C0                     "C0"
#define C1                     "C1"
#define C2                     "C2"
#define C3                     "C3"
#define C4                     "C4"
#define C5                     "C5"
#define C6                     "C6"
#define C7                     "C7"
#define C8                     "C8"
#define C9                     "C9"
#define C10                    "C10"
#define C11                    "C11"
#define C12                    "C12"
#define C13                    "C13"
#define C14                    "C14"
#define C15                    "C15"

#define RCC_APB2Periph_GPIOx(x)           \
    (x[0] == 'A'   ? RCC_APB2Periph_GPIOA \
     : x[0] == 'B' ? RCC_APB2Periph_GPIOB \
     : x[0] == 'C' ? RCC_APB2Periph_GPIOC \
                   : NULL)

#define GPIOx(x) \
    (x[0] == 'A' ? GPIOA : x[0] == 'B' ? GPIOB : x[0] == 'C' ? GPIOC : NULL)

#define GPIO_Pin(x)  (x[2] ? 10 + x[2] - '0' : x[1] - '0')
#define GPIO_Pinx(x) GPIO_Pin_0 << GPIO_Pin(x)

#define GPIO_Input(CR, x)                                         \
    do {                                                          \
        MEM_ADDR((CR)) &= (~((uint32_t)0b1111 << ((x) % 8 * 4))); \
        MEM_ADDR((CR)) |= ((uint32_t)0b1000 << ((x) % 8 * 4));    \
    } while (0)

#define GPIO_Output(CR, x)                                        \
    do {                                                          \
        MEM_ADDR((CR)) &= (~((uint32_t)0b1111 << ((x) % 8 * 4))); \
        MEM_ADDR((CR)) |= ((uint32_t)0b0011 << ((x) % 8 * 4));    \
    } while (0)

#define GPIO_Write(x, val) MEM_ADDR((x)) = ((val) ? 1 : 0)
#define GPIO_Toggle(x)     MEM_ADDR((x)) = !MEM_ADDR((x))
#define GPIO_ReadInput(x)  MEM_ADDR((x))
#define GPIO_ReadOutput(x) MEM_ADDR((x))

typedef char GPIOxPiny_t[4];

typedef struct {
    char             GPIOxPiny[4];
    GPIOMode_TypeDef Mode;
} GPIO_t;

uint32_t GPIO_InitPin(GPIO_t *GPIO, const GPIOxPiny_t Pin);

uint32_t GPIO_CR(const GPIOxPiny_t Pin);
uint32_t GPIO_IDR(const GPIOxPiny_t Pin);
uint32_t GPIO_ODR(const GPIOxPiny_t Pin);

#endif