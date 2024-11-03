#ifndef GPIO_H
#define GPIO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define BITBAND(addr, bitnum)                                                  \
    ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)         *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr         (GPIOA_BASE + 12)
#define GPIOB_ODR_Addr         (GPIOB_BASE + 12)
#define GPIOC_ODR_Addr         (GPIOC_BASE + 12)
#define GPIOD_ODR_Addr         (GPIOD_BASE + 12)
#define GPIOE_ODR_Addr         (GPIOE_BASE + 12)
#define GPIOF_ODR_Addr         (GPIOF_BASE + 12)
#define GPIOG_ODR_Addr         (GPIOG_BASE + 12)

#define GPIOA_IDR_Addr         (GPIOA_BASE + 8)
#define GPIOB_IDR_Addr         (GPIOB_BASE + 8)
#define GPIOC_IDR_Addr         (GPIOC_BASE + 8)
#define GPIOD_IDR_Addr         (GPIOD_BASE + 8)
#define GPIOE_IDR_Addr         (GPIOE_BASE + 8)
#define GPIOF_IDR_Addr         (GPIOF_BASE + 8)
#define GPIOG_IDR_Addr         (GPIOG_BASE + 8)

#define PAout(n)               BIT_ADDR(GPIOA_ODR_Addr, n)
#define PAin(n)                BIT_ADDR(GPIOA_IDR_Addr, n)

#define PBout(n)               BIT_ADDR(GPIOB_ODR_Addr, n)
#define PBin(n)                BIT_ADDR(GPIOB_IDR_Addr, n)

#define PCout(n)               BIT_ADDR(GPIOC_ODR_Addr, n)
#define PCin(n)                BIT_ADDR(GPIOC_IDR_Addr, n)

#define PDout(n)               BIT_ADDR(GPIOD_ODR_Addr, n)
#define PDin(n)                BIT_ADDR(GPIOD_IDR_Addr, n)

#define PEout(n)               BIT_ADDR(GPIOE_ODR_Addr, n)
#define PEin(n)                BIT_ADDR(GPIOE_IDR_Addr, n)

#define PFout(n)               BIT_ADDR(GPIOF_ODR_Addr, n)
#define PFin(n)                BIT_ADDR(GPIOF_IDR_Addr, n)

#define PGout(n)               BIT_ADDR(GPIOG_ODR_Addr, n)
#define PGin(n)                BIT_ADDR(GPIOG_IDR_Addr, n)

#define RCC_APB2Periph_GPIOx(x)                                                \
    ((x[0]) == 'A'   ? RCC_APB2Periph_GPIOA                                    \
     : (x[0]) == 'B' ? RCC_APB2Periph_GPIOB                                    \
     : (x[0]) == 'C' ? RCC_APB2Periph_GPIOC                                    \
                     : NULL)

#define GPIOx(x)                                                               \
    ((x[0]) == 'A'   ? GPIOA                                                   \
     : (x[0]) == 'B' ? GPIOB                                                   \
     : (x[0]) == 'C' ? GPIOC                                                   \
                     : NULL)

#define GPIO_Pinx(x)                                                           \
    ((x[1]) == '0'   ? GPIO_Pin_0                                              \
     : (x[2]) == '5' ? GPIO_Pin_15                                             \
     : (x[2]) == '4' ? GPIO_Pin_14                                             \
     : (x[2]) == '3' ? GPIO_Pin_13                                             \
     : (x[2]) == '2' ? GPIO_Pin_12                                             \
     : (x[2]) == '1' ? GPIO_Pin_11                                             \
     : (x[2]) == '0' ? GPIO_Pin_10                                             \
     : (x[1]) == '9' ? GPIO_Pin_9                                              \
     : (x[1]) == '8' ? GPIO_Pin_8                                              \
     : (x[1]) == '7' ? GPIO_Pin_7                                              \
     : (x[1]) == '6' ? GPIO_Pin_6                                              \
     : (x[1]) == '5' ? GPIO_Pin_5                                              \
     : (x[1]) == '4' ? GPIO_Pin_4                                              \
     : (x[1]) == '3' ? GPIO_Pin_3                                              \
     : (x[1]) == '2' ? GPIO_Pin_2                                              \
     : (x[1]) == '1' ? GPIO_Pin_1                                              \
                     : NULL)

#define A0  "A0"
#define A1  "A1"
#define A2  "A2"
#define A3  "A3"
#define A4  "A4"
#define A5  "A5"
#define A6  "A6"
#define A7  "A7"
#define A8  "A8"
#define A9  "A9"
#define A10 "A10"
#define A11 "A11"
#define A12 "A12"
#define A13 "A13"
#define A14 "A14"
#define A15 "A15"

#define B0  "B0"
#define B1  "B1"
#define B2  "B2"
#define B3  "B3"
#define B4  "B4"
#define B5  "B5"
#define B6  "B6"
#define B7  "B7"
#define B8  "B8"
#define B9  "B9"
#define B10 "B10"
#define B11 "B11"
#define B12 "B12"
#define B13 "B13"
#define B14 "B14"
#define B15 "B15"

#define C0  "C0"
#define C1  "C1"
#define C2  "C2"
#define C3  "C3"
#define C4  "C4"
#define C5  "C5"
#define C6  "C6"
#define C7  "C7"
#define C8  "C8"
#define C9  "C9"
#define C10 "C10"
#define C11 "C11"
#define C12 "C12"
#define C13 "C13"
#define C14 "C14"
#define C15 "C15"

typedef struct {
    char GPIOxPiny[32];
    GPIOMode_TypeDef Mode;

    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
} GPIO_t;

void GPIO_Init_(GPIO_t *self);

#endif