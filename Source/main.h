#include "FreeRTOS.h"
#include "timers.h"

#include "Encoder.h"
#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "MQSensor.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"
#include "W25Q64.h"

#define MAX(a, b)             ((a) > (b) ? (a) : (b))
#define MIN(a, b)             ((a) < (b) ? (a) : (b))

#define ADCToVoltage(ADC)     ((ADC) / 4095.0 * 3.3)
#define VoltageToADC(Voltage) (uint32_t)((Voltage) / 3.3 * 4095.0)

extern LED_t LED;
extern Key_t Key;
extern W25Q64_t W25Q64;

extern Key_t KeyConfirm;
extern Encoder_t Encoder;

extern MQSensor_t MQSensor[4];

extern Sampler_t Sampler;

extern OLED_t OLED;

extern TextPage_t MonitorPage;
extern TextPage_t MQxChartPage[5];
extern TextPage_t SettingPage;

extern ImagePage_t HomePage[2];

extern void *Menu;
extern TextMenu_t TextMenu;
extern ImageMenu_t ImageMenu;
extern SelectioneBar_t Bar;

extern TextPage_t *StatusLEDSetting;
extern TextPage_t *ReverseSetting;
extern TextPage_t *RestartSetting;

extern TaskHandle_t xMenuKeyTaskHandle;
extern void vMenuKeyTaskCode(void *pvParameters);

extern TimerHandle_t vUpdateTimer;
extern TimerHandle_t vOLEDTimer;
extern void vUpdateTimerCallback(TimerHandle_t pxTimer);
extern void vOLEDTimerCallback(TimerHandle_t pxTimer);