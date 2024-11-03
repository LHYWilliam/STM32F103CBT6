# LED
```
LED_t LED = {
    .GPIOxPiny = B2,
    .Mode = HIGH,
};

LED_Init(&LED);
```


# Key
```
Key_t Key = {
    .GPIOxPiny = A0,
    .Mode = HIGH,
};

Key_Init(&Key);
```


# OLED
```
OLED_t OLED = {
    .SCL = B8,
    .SDA = B9,
};

OLED_Init(&OLED);
```


# Motor
```
Motor_t Motor = {
    .PWM = A8,
    .IN1 = A9,
    .IN2 = A10,
    .TIMx = TIM1,
    .Channel = 1,
    .TIM_Init = ENABLE,
};

Motor_Init(&Motor);
```


# Sampler
## 软件单通道
```
Sampler_t Sampler = {
    .ADCx = ADC1,
    .ADC_Channel = "1",
    .GPIOxPiny = "A1",
    .Continuous = ENABLE,
};

Sampler_Init(&Sampler);

Sampler_Get(&Sampler, 1);
```

## 软件多通道
```
Sampler_t Sampler = {
    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",
    .Continuous = DISABLE,
};

Sampler_Init(&Sampler);


Sampler_Get(&Sampler, 1);
Sampler_Get(&Sampler, 2);
```

# DMA多通道
```
#define LENGTH 2
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .Continuous = ENABLE,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

    .DMAx = DMA1,
    .DMA_Channel = 1,
};

Sampler_Init(&Sampler);

Sampler.Data[0]
Sampler.Data[1]
```

# Timer
```
Timer_t Timer = {
    .TIMx = TIM2,
    .ms = 1000,
};

Timer_Init(&Timer);

void TIM?_IRQHandler() {
    if (TIM_GetITStatus(Timer.TIMx, TIM_IT_Update)) {

        TIM_ClearITPendingBit(Timer.TIMx, TIM_IT_Update);
    }
}
```