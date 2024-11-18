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
### Software I2C
```
OLED_t OLED = {
    .SCL = B6,
    .SDA = B7,
    .I2C = ENABLE,
};
```

### Software SPI
```
OLED_t OLED = {
    .D0 = B13,
    .D1 = B15,
    .RES = A8,
    .DC = A9,
    .CS = A10,
    .SPI = ENABLE,
};
```

### Hardware I2C
```
OLED_t OLED = {
    .SCL = B6,
    .SDA = B7,
    .I2Cx = I2C1,
};
```

### Hardware SPI
```
OLED_t OLED = {
    .D0 = B13,
    .D1 = B15,
    .RES = A8,
    .DC = A9,
    .CS = A10,
    .SPIx = SPI2,
};
```

### Software I2C U8g2
```
OLED_t OLED = {
    .SCL = B6,
    .SDA = B7,
    .Width = 128,
    .Height = 64,
    .I2C = ENABLE,
    .U8g2 = ENABLE,
};
```

### Software SPI U8g2
```
OLED_t OLED = {
    .D0 = B13,
    .D1 = B15,
    .RES = A8,
    .DC = A9,
    .CS = A10,
    .Width = 128,
    .Height = 64,
    .SPI = ENABLE,
    .U8g2 = ENABLE,
};
```

### Hardware I2C U8g2
```
OLED_t OLED = {
    .SCL = B6,
    .SDA = B7,
    .Width = 128,
    .Height = 64,
    .I2Cx = I2C1,
    .U8g2 = ENABLE,
};
```

### Hardware SPI U8g2
```
OLED_t OLED = {
    .D0 = B13,
    .D1 = B15,
    .RES = A8,
    .DC = A9,
    .CS = A10,
    .Width = 128,
    .Height = 64,
    .SPIx = SPI2,
    .U8g2 = ENABLE,
};
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
### Soft
```
Sampler_t Sampler = {
    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",
};

Sampler_Init(&Sampler);


Sampler_Get(&Sampler, 1);
Sampler_Get(&Sampler, 2);
```

### DMA
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

### DMA + TIM
```
#define LENGTH 2
uint16_t Data[LENGTH];

Sampler_t Sampler = {
    .Data = Data,
    .Length = LENGTH,

    .ADCx = ADC1,
    .ADC_Channel = "1 | 2",
    .GPIOxPiny = "A1 | A2",

    .DMAx = DMA1,
    .DMA_Channel = 1,

    .TIMx = TIM3,
    .Hz = 10,
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