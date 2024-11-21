#include "FreeRTOS.h"
#include "timers.h"

#include "GPIO.h"
#include "Key.h"
#include "LED.h"
#include "Menu.h"
#include "OLED.h"
#include "Sampler.h"

#define MQx_DataLength 128

extern LED_t LED;

extern Sampler_t Sampler;

extern TextMenu_t Menu;
extern OLED_t OLED;

extern TextPage_t *HomePage;
extern TextPage_t *MQ3Page;
extern TextPage_t *MQ135Page;

extern LED_t MQ3_LED;
extern LED_t MQ135_LED;
extern Key_t KeyUp;
extern Key_t KeyDown;
extern Key_t KeyConfirm;
extern Key_t KeyCancel;

extern int16_t MQ3_Index;
extern uint16_t MQ3_Data[128];

extern int16_t MQ135_Index;
extern uint16_t MQ135_Data[128];

static void OLED_ShowHomePage(OLED_t *OLED);
static void OLED_ShowMQxMenu(OLED_t *OLED, TextPage_t *MQxPage,
                             uint16_t *MQx_Data, int16_t MQx_Index,
                             LED_t *MQx_LED, uint8_t begin, uint8_t i);
static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             uint16_t *MQx_Data, int16_t MQx_Index,
                             LED_t *MQx_LED);

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    if (MQ3_Data[MQ3_Index] > 2048 + 256) {
        LED_On(&MQ3_LED);
    } else if (MQ3_Data[MQ3_Index] < 2048 - 256) {
        LED_Off(&MQ3_LED);
    }

    if (MQ135_Data[MQ135_Index] > 2048 + 256) {
        LED_On(&MQ135_LED);
    } else if (MQ135_Data[MQ135_Index] < 2048 - 256) {
        LED_Off(&MQ135_LED);
    }

    LED_Turn(&LED);
}

void vOLEDTimerCallback(TimerHandle_t pxTimer) {
    OLED_ClearBuffer(&OLED);

    static uint32_t time;
    OLED_Printf(&OLED, OLED.Width - OLED.FontWidth * 4 - 1,
                OLED.Height - OLED.FontHeight - 1, "%2d %%", time);
    time = xTaskGetTickCount();

    if (Menu.Page == HomePage) {
        OLED_ShowHomePage(&OLED);

    } else if (Menu.Page == MQ3Page) {
        OLED_ShowMQxPage(&OLED, MQ3Page, MQ3_Data, MQ3_Index, &MQ3_LED);

    } else if (Menu.Page == MQ135Page) {
        OLED_ShowMQxPage(&OLED, MQ135Page, MQ135_Data, MQ135_Index, &MQ135_LED);

    } else {
        uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;
        for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {

            OLED_Printf(&OLED, 0, 20 + i * (OLED.FontHeight + 2), "%s%s%s",
                        begin + i == Menu.Cursor ? ">" : ".",
                        Menu.Page->LowerPages[begin + i].Title,
                        begin + i == Menu.Cursor ? "<" : "");
        }
    }

    OLED_SendBuffer(&OLED);

    time = xTaskGetTickCount() - time;
}

void vMenuKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&KeyUp)) {
            TextMenu_CursorDec(&Menu);
        }
        if (Key_Read(&KeyDown)) {
            TextMenu_CursorInc(&Menu);
        }
        if (Key_Read(&KeyConfirm)) {
            TextMenu_EnterLowerPage(&Menu);
        }
        if (Key_Read(&KeyCancel)) {
            TextMenu_ReturnUpperPage(&Menu);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void OLED_ShowHomePage(OLED_t *OLED) {
    OLED_SetFont(OLED, OLEDFont_Chinese12X12);
    OLED_Printf(OLED, 10 + 1 - 1, 1 - 1, "异味检测与开窗系统");
    OLED_SetFont(OLED, OLEDFont_6X8);

    uint8_t begin = Menu.Cursor >= TEXT_COUNT_OF_PAGE ? Menu.Cursor - 3 : 0;
    for (uint8_t i = 0; i < Menu.Page->NumOfLowerPages; i++) {
        if (&Menu.Page->LowerPages[begin + i] == MQ3Page) {
            OLED_ShowMQxMenu(OLED, MQ3Page, MQ3_Data, MQ3_Index, &MQ3_LED,
                             begin, i);

        } else if (&Menu.Page->LowerPages[begin + i] == MQ135Page) {
            OLED_ShowMQxMenu(OLED, MQ135Page, MQ135_Data, MQ135_Index,
                             &MQ135_LED, begin, i);

        } else {
            OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s%s%s",
                        begin + i == Menu.Cursor ? ">" : ".",
                        Menu.Page->LowerPages[begin + i].Title,
                        begin + i == Menu.Cursor ? "<" : "");
        }
    }
}

static void OLED_ShowMQxMenu(OLED_t *OLED, TextPage_t *MQxPage,
                             uint16_t *MQx_Data, int16_t MQx_Index,
                             LED_t *MQx_LED, uint8_t begin, uint8_t i) {
    OLED_Printf(OLED, 0, 20 + i * (OLED->FontHeight + 2), "%s%-6s %.3f%s",
                begin + i == Menu.Cursor ? ">" : ".",
                Menu.Page->LowerPages[begin + i].Title,
                MQx_Data[MQx_Index] * 3.3 / 4095.,
                begin + i == Menu.Cursor ? "<" : "");
    OLED_Printf(OLED, OLED->Width - OLED->FontWidth * 6,
                20 + i * (OLED->FontHeight + 2), "%6s",
                GPIO_ReadInput(MQx_LED->ODR) == MQx_LED->Mode ? "Danger"
                                                              : "Safe");
}

static void OLED_ShowMQxPage(OLED_t *OLED, TextPage_t *MQxPage,
                             uint16_t *MQx_Data, int16_t MQx_Index,
                             LED_t *MQx_LED) {
    for (uint16_t x = 0, Index = (MQx_Index + 1) % MQx_DataLength;
         x < OLED->Width - 1; x++, Index = (Index + 1) % MQx_DataLength) {
        OLED_DrawLine(OLED, x * (OLED->Width - 1) / (MQx_DataLength - 1),
                      OLED->Height - 1 -
                          (MQx_Data[Index] * (OLED->Height - 1) / 2. / 4095. +
                           (OLED->Height - 1) / 4.),
                      (x + 1) * (OLED->Width - 1) / (MQx_DataLength - 1),
                      OLED->Height - 1 -
                          (MQx_Data[(Index + 1) % MQx_DataLength] *
                               (OLED->Height - 1) / 2. / 4095. +
                           (OLED->Height - 1) / 4.));
    }

    OLED_Printf(OLED, 1 - 1, 1 - 1, "%S %s", MQxPage->Title,
                GPIO_ReadInput(MQx_LED->ODR) == MQx_LED->Mode ? "Danger"
                                                              : "Safe");

    OLED_Printf(OLED, 1 - 1, OLED->Height - OLED->FontHeight - 1, "%.3f V",
                MQx_Data[MQx_Index] * 3.3 / 4095.);
}