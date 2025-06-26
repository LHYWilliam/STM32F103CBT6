#include "main.h"

void vMainTaskCode(void *pvParameters) {
    float YawPitchRoll[3];

    for (;;) {
        ICM42688_AHRS_Update(&ICM42688);
        ICM42688_AHRS_GetYawPitchRoll(&ICM42688, YawPitchRoll);

        printf("Yaw, Pitch, Roll :  %6.2f, %6.2f, %6.2f\n ", YawPitchRoll[0],
               YawPitchRoll[1], YawPitchRoll[2]);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED); }
