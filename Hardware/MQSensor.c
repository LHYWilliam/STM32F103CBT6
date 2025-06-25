#include "MQSensor.h"

void MQSensor_Init(MQSensor_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;
    Self->ODR = GPIO_InitPin(&GPIO, Self->LED);

    GPIO_Write(Self->ODR, !Self->Mode);

    Self->Length = MQSensor_DataLength;
    Self->Index  = -1;
}

void MQSensor_UpdateState(MQSensor_t *Self) {
    if (Self->Data[Self->Index] > Self->Threshold + Self->Relaxation) {
        GPIO_Write(Self->ODR, Self->Mode);
        Self->State = MQSensorState_Warning;

    } else if (Self->Data[Self->Index] < Self->Threshold - Self->Relaxation) {
        GPIO_Write(Self->ODR, !Self->Mode);
        Self->State = MQSensorState_Safe;
    }
}

void MQSensor_UpdateThreshold(MQSensor_t *Self, int16_t dt) {
    if ((0 <= Self->Threshold + dt) && (Self->Threshold + dt <= 4095)) {
        Self->Threshold += dt;
    }
}