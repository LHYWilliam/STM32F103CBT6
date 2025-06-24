#include "FreeRTOS.h"
#include "timers.h"

#include "Encoder.h"
#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "MQSensor.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"
#include "Serial.h"
#include "W25Q64.h"

#define ADCToVoltage(ADC)     ((ADC) / 4095.0 * 3.3)
#define VoltageToADC(Voltage) (uint32_t)((Voltage) / 3.3 * 4095.0)

extern LED_t      LED;
extern Encoder_t  Encoder;
extern Key_t      KeyConfirm;
extern LCD_t      LCD;
extern OLED_t     OLED;
extern Serial_t   Serial;
extern W25Q64_t   W25Q64;
extern Sampler_t  Sampler;
extern MQSensor_t MQSensor[4];

extern TextPage_t      MonitorPage;
extern TextPage_t      SettingPage;
extern TextPage_t     *LEDSetting;
extern TextPage_t     *ReverseSetting;
extern void           *Menu;
extern TextMenu_t      TextMenu;
extern ImageMenu_t     ImageMenu;
extern SelectioneBar_t Bar;

extern void SettingLoad(void *pvParameters);

extern TaskHandle_t xMenuKeyTaskHandle;
extern void         vMenuKeyTaskCode(void *pvParameters);

extern TimerHandle_t vUpdateTimer;
extern TimerHandle_t vOLEDTimer;
extern void          vUpdateTimerCallback(TimerHandle_t pxTimer);
extern void          vOLEDTimerCallback(TimerHandle_t pxTimer);

extern void TextPage_UpdateCallback(void *pvParameters);
extern void TextPage_UpdateDialogCallback(void *pvParameters);

extern void TextPage_ShowMQxCallback(void *pvParameters);
extern void TextPage_ShowDialogCallback(void *pvParameters);
extern void TextPage_ShowMonitorCallback(void *pvParameters);
extern void TextPage_ShowSettingCallback(void *pvParameters);
extern void ImagePage_ShowCallback(void *pvParameters);

extern void TextPage_BackCallback(void *pvParameters);
extern void TextPage_EnterCallback(void *pvParameters);
extern void TextPage_SaveSettingCallback(void *pvParameters);
extern void TextPage_LoadSettingCallback(void *pvParameters);
extern void TextPage_ReverseSettingCallback(void *pvParameters);
extern void TextPage_RestartSettingCallback(void *pvParameters);
extern void TextPage_CursorSwitchEncoderSettingCallback(void *pvParameters);
extern void TextPage_BackImageMenuCallback(void *pvParameters);
extern void ImagePage_EnterTextPageCallback(void *pvParameters);

extern void TextPage_CursorCallback(int16_t Encoder);
extern void TextPage_ThresholdCallback(int16_t Encoder);
extern void TextPage_SettingEncoderCallback(int16_t Encoder);
extern void ImagePage_CursorCallback(int16_t Encoder);
