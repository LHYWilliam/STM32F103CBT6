#include "MQSensor.h"

void MQSensor_Init(MQSensor_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;
    GPIO_InitPin(&GPIO, self->LED);

    self->ODR = GPIO_ODR(self->LED);

    GPIO_Write(self->ODR, !self->Mode);

    self->Length = MQSensor_DataLength;
    self->Index = -1;
}

void MQSensor_UpdateState(MQSensor_t *self) {
    if (self->Data[self->Index] > self->Threshold + self->Relaxation) {
        GPIO_Write(self->ODR, self->Mode);
        self->State = MQSensorState_Warning;

    } else if (self->Data[self->Index] < self->Threshold - self->Relaxation) {
        GPIO_Write(self->ODR, !self->Mode);
        self->State = MQSensorState_Safe;
    }
}

void MQSensor_UpdateThreshold(MQSensor_t *self, int16_t dt) {
    if ((0 <= self->Threshold + dt) && (self->Threshold + dt <= 4095)) {
        self->Threshold += dt;
    }
}