#include "RTE_Components.h"
#include CMSIS_device_header

#include "Delay.h"

void Delay_us(uint32_t us) {
    uint32_t now, last, reload, total, goal;
    total = 0;
    goal = us * 72;
    last = SysTick->VAL;
    reload = SysTick->LOAD;

    for (;;) {
        now = SysTick->VAL;
        if (now != last) {
            total += now < last ? last - now : reload - now + last;

            if (total >= goal) {
                break;
            }

            last = now;
        }
    }
}

void Delay_ms(uint32_t ms) {
    while (ms--) {
        Delay_us(1000);
    }
}