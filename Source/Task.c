#include "main.h"

void vMainTaskCode(void *pvParameters) {
    // float YawPitchRoll[3];
    // for (;;) {
    //     ICM42688_AHRS_Update(&ICM42688);
    //     ICM42688_AHRS_GetYawPitchRoll(&ICM42688, YawPitchRoll);

    //     printf("Yaw, Pitch, Roll :  %6.2f, %6.2f, %6.2f\n ", YawPitchRoll[0],
    //            YawPitchRoll[1], YawPitchRoll[2]);

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    uint8_t GrayScaleData[8];
    for (;;) {
        GrayScaleSensor_ReadAnalog(&GrayScaleSensor, GrayScaleData);

        printf("%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d\n", GrayScaleData[0],
               GrayScaleData[1], GrayScaleData[2], GrayScaleData[3],
               GrayScaleData[4], GrayScaleData[5], GrayScaleData[6],
               GrayScaleData[7]);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED); }
