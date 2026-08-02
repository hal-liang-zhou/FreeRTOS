#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#define TASK_STK_SIZE 128U

static SemaphoreHandle_t xTickSemaphore = NULL;

static void vTaskTickHandler(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xSemaphoreTake(xTickSemaphore, portMAX_DELAY) == pdTRUE) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "Timer IRQ -> task: tick\n");
        }
    }
}

void TIM3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        (void)xSemaphoreGiveFromISR(xTickSemaphore, &xHigherPriorityTaskWoken);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

static void vInitTimer(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 7199U;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 999U;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0U;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

void main(void) {
    vGpio_Init();
    vInitTimer();
    xTickSemaphore = xSemaphoreCreateBinary();
    (void)xTaskCreate(vTaskTickHandler, "Tick", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}
