#include "gpiodef.h"
#include "main.h"
#include "stddef.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

typedef struct {
    char *des;
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t clkPeriph;
    GPIOMode_TypeDef mode;
    uint8_t initialState;
    GPIOSpeed_TypeDef speed;
    uint32_t remap;
} sGpioInfo_t;

static const sGpioInfo_t sGpioInfo[] = {
    {"DBG_LED", GPIOA, GPIO_Pin_8, RCC_APB2Periph_GPIOA, GPIO_Mode_Out_PP, 0, GPIO_Speed_50MHz, 0},
};

void vGpio_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    int i;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                               RCC_APB2Periph_GPIOE,
                           ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                               RCC_APB2Periph_GPIOE,
                           DISABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    for (i = 0; i < sizeof(sGpioInfo) / sizeof(sGpioInfo_t); i++) {
        RCC_APB2PeriphClockCmd(sGpioInfo[i].clkPeriph, ENABLE);

        if (sGpioInfo[i].mode == GPIO_Mode_Out_OD || sGpioInfo[i].mode == GPIO_Mode_Out_PP) {
            GPIO_WriteBit(sGpioInfo[i].port, sGpioInfo[i].pin, sGpioInfo[i].initialState == 1 ? SET : RESET);
        }

        GPIO_InitStructure.GPIO_Pin = sGpioInfo[i].pin;
        GPIO_InitStructure.GPIO_Mode = sGpioInfo[i].mode;
        GPIO_InitStructure.GPIO_Speed = sGpioInfo[i].speed;

        GPIO_Init(sGpioInfo[i].port, &GPIO_InitStructure);

        if (sGpioInfo[i].remap) {
            GPIO_PinRemapConfig(sGpioInfo[i].remap, ENABLE);
        }
    }
}
