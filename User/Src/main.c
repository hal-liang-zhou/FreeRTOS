#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#define TASK_STK_SIZE 128U

static SemaphoreHandle_t xButtonSemaphore = NULL;

static void vTaskButtonHandler(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "Button pressed, toggle LED\n");
        }
    }
}

void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        (void)xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

static void vInitButton(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0U;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void main(void) {
    vGpio_Init();
    vInitButton();
    xButtonSemaphore = xSemaphoreCreateBinary();
    (void)xTaskCreate(vTaskButtonHandler, "Button", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}
