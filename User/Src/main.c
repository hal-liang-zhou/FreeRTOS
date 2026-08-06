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

static SemaphoreHandle_t xSyncSemaphore = NULL;
static QueueHandle_t xSyncQueue = NULL;
static TaskHandle_t xNotifiedTask = NULL;

static uint32_t ulNotifyCount = 0U;

static void vTaskSemWait(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xSemaphoreTake(xSyncSemaphore, portMAX_DELAY) == pdTRUE) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "[Semaphore] IRQ->task: woke up\n");
        }
    }
}

static void vTaskQueueRecv(void *pvParameters) {
    (void)pvParameters;
    uint32_t ulReceived;
    for (;;) {
        if (xQueueReceive(xSyncQueue, &ulReceived, portMAX_DELAY) == pdTRUE) {
            DEBUG_INFO(0U, "[Queue] IRQ->task: got count=%lu\n",
                       (unsigned long)ulReceived);
        }
    }
}

static void vTaskNotifyWait(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xTaskNotifyWait((uint32_t)0, (uint32_t)0, NULL, portMAX_DELAY) == pdTRUE) {
            ulNotifyCount++;
            DEBUG_INFO(0U, "[Notification] IRQ->task: event #%lu\n",
                       (unsigned long)ulNotifyCount);
        }
    }
}

void TIM3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulCount = 0U;

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        ulCount++;

        (void)xSemaphoreGiveFromISR(xSyncSemaphore, &xHigherPriorityTaskWoken);

        (void)xQueueSendFromISR(xSyncQueue, &ulCount, &xHigherPriorityTaskWoken);

        (void)xTaskNotifyFromISR(xNotifiedTask, (uint32_t)0,
                                 eNoAction, &xHigherPriorityTaskWoken);

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
    TIM_TimeBaseStructure.TIM_Period = 4999U;
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

    xSyncSemaphore = xSemaphoreCreateBinary();
    xSyncQueue = xQueueCreate(8U, sizeof(uint32_t));

    (void)xTaskCreate(vTaskSemWait, "Sem", TASK_STK_SIZE, NULL, 3U, NULL);
    (void)xTaskCreate(vTaskQueueRecv, "Que", TASK_STK_SIZE, NULL, 2U, NULL);
    (void)xTaskCreate(vTaskNotifyWait, "Ntf", TASK_STK_SIZE, NULL, 1U, &xNotifiedTask);

    vTaskStartScheduler();
}