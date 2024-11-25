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

extern MQSensor_t MQ2Sensor;
extern MQSensor_t MQ3Sensor;
extern MQSensor_t MQ7Sensor;
extern MQSensor_t MQ135Sensor;

extern Sampler_t Sampler;

extern TextMenu_t TextMenu;
extern ImageMenu_t ImageMenu;
extern SelectioneBar_t Bar;

extern void *Menu;

extern TextPage_t *HomeTextPage;
extern TextPage_t *MQ2TextPage;
extern TextPage_t *MQ3TextPage;
extern TextPage_t *MQ7TextPage;
extern TextPage_t *MQ135TextPage;
extern TextPage_t *SettingTextPage;

extern TextPage_t *StatusLEDSetting;
extern TextPage_t *ReverseSetting;
extern TextPage_t *RestartSetting;

extern TaskHandle_t xMenuKeyTaskHandle;
extern void vMenuKeyTaskCode(void *pvParameters);

extern TimerHandle_t vStateTimer;
extern TimerHandle_t vOLEDTimer;
extern void vStateTimerCallback(TimerHandle_t pxTimer);
extern void vOLEDTimerCallback(TimerHandle_t pxTimer);