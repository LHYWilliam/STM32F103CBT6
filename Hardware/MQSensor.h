#ifndef MQSENSOR_H
#define MQSENSOR_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"
#include "LED.h"

#define MQSensor_DataLength 128

typedef enum {
    MQSensorState_Safe,
    MQSensorState_Warning,
} MQSensorState;

typedef struct {
    GPIOxPiny_t LED;
    LEDMode Mode;
    uint32_t ODR;

    uint16_t Threshold;
    uint16_t Relaxation;
    MQSensorState State;

    int16_t Index;
    uint16_t Length;
    uint16_t Data[MQSensor_DataLength];
} MQSensor_t;

void MQSensor_Init(MQSensor_t *self);
void MQSensor_UpdateState(MQSensor_t *self);
void MQSensor_UpdateThreshold(MQSensor_t *self, int16_t dt);

#endif