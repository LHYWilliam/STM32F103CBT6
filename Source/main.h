#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "MQSensor.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

#define MAX(a, b)             ((a) > (b) ? (a) : (b))
#define MIN(a, b)             ((a) < (b) ? (a) : (b))

#define ADCToVoltage(ADC)     ((ADC) / 4095.0 * 3.3)
#define VoltageToADC(Voltage) (uint32_t)((Voltage) / 3.3 * 4095.0)

extern LED_t LED;

extern Key_t Key;
extern Key_t KeyUp;
extern Key_t KeyDown;
extern Key_t KeyConfirm;
extern Key_t KeyCancel;

extern OLED_t OLED;

extern MQSensor_t MQ2;
extern MQSensor_t MQ3;
extern MQSensor_t MQ7;
extern MQSensor_t MQ135;

extern Sampler_t Sampler;

extern TextMenu_t Menu;
extern TextPage_t *HomePage;
extern TextPage_t *MQ2Page;
extern TextPage_t *MQ3Page;
extern TextPage_t *MQ7Page;
extern TextPage_t *MQ135Page;
extern TextPage_t *SettingPage;

extern SelectioneBar_t Bar;

extern TextPage_t *StatusLEDSetting;
extern TextPage_t *ReverseSetting;
extern TextPage_t *RestartSetting;

extern TaskHandle_t xMenuKeyTaskHandle;
extern void vMenuKeyTaskCode(void *pvParameters);

extern TimerHandle_t vStateTimer;
extern TimerHandle_t vOLEDTimer;
extern void vStateTimerCallback(TimerHandle_t pxTimer);
extern void vOLEDTimerCallback(TimerHandle_t pxTimer);