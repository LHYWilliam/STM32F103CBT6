#ifndef RTC_H
#define RTC_H

#include "RTE_Components.h"
#include CMSIS_device_header

void     RTC_Init(void);
uint32_t RTC_time_s(void);
uint32_t RTC_time_ms(void);

#endif