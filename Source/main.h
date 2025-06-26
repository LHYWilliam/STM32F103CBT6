#include "FreeRTOS.h"
#include "timers.h"

#include "Delay.h"
#include "RTC.h"

#include "Key.h"
#include "LED.h"
#include "Serial.h"

// #include "Encoder.h"
// #include "Sampler.h"

// #include "OLED.h"
// #include "LCD.h"

// #include "W25Q64.h"
// #include "MQSensor.h"

#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "ICM42688_SPI.h"

extern LED_t    LED;
extern Key_t    Key;
extern Serial_t Serial;

// extern OLED_t     OLED;
// extern LCD_t      LCD;

// extern Encoder_t Encoder;
// extern Sampler_t  Sampler;

// extern W25Q64_t   W25Q64;
// extern MQSensor_t MQSensor[4];

extern ICM42688_t ICM42688;

extern TaskHandle_t xMainTaskHandle;

extern void vMainTaskCode(void *pvParameters);

extern TimerHandle_t vLEDTimer;

extern void vLEDTimerCallback(TimerHandle_t pxTimer);
