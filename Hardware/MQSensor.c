#include "MQSensor.h"
#include "GPIO.h"

void MQSensor_Init(MQSensor_t *self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_Mode_Out_PP;
    GPIO_InitPin(&GPIO, self->LED);

    self->ODR = GPIO_ODR(self->LED);

    GPIO_Write(self->ODR, !self->Mode);

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