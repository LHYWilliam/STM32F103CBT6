#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "MQSensor.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

extern LED_t LED;

extern Key_t Key;
extern Key_t KeyUp;
extern Key_t KeyDown;
extern Key_t KeyConfirm;
extern Key_t KeyCancel;

extern OLED_t OLED;

extern MQSensor_t MQ3;
extern MQSensor_t MQ135;

extern Sampler_t Sampler;

extern TextMenu_t Menu;
extern TextPage_t *HomePage;
extern TextPage_t *MQ3Page;
extern TextPage_t *MQ135Page;

extern TaskHandle_t xMenuKeyTaskHandle;
void vMenuKeyTaskCode(void *pvParameters);

extern TimerHandle_t vLEDTimer;
extern TimerHandle_t vOLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);
void vOLEDTimerCallback(TimerHandle_t pxTimer);