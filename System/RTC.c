#include "RTE_Components.h"
#include CMSIS_device_header

#include "RTC.h"

void RTC_Init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    // RCC_LSEConfig(RCC_LSE_ON);
    // while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET)
    //     ;
    // RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    // RCC_RTCCLKCmd(ENABLE);

    // RTC_WaitForSynchro();
    // RTC_WaitForLastTask();

    // RTC_SetPrescaler(32768 - 1);
    // RTC_WaitForLastTask();

    RCC_LSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET)
        ;

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    RTC_SetPrescaler(40000 - 1);
    RTC_WaitForLastTask();

    RTC_SetCounter(0);
    RTC_WaitForLastTask();
}

uint32_t RTC_Gets() { return RTC_GetCounter(); }

uint32_t RTC_Getms() {
    return RTC_GetCounter() * 1000 + (40000 - RTC_GetDivider()) / 40000.0 * 999;
}

uint32_t RTC_Getus() {
    return RTC_Getms() * 1000 +
           (SysTick->LOAD + 1 - SysTick->VAL) * 1000 / (SysTick->LOAD + 1);
}