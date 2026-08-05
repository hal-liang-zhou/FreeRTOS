#include "FreeRTOS.h"
#include "debug_config.h"
#include "gpiodef.h"
#include "gpiox.h"
#include "task.h"

#define TASK_STK_SIZE 256U
#define MAX_TASKS     8U

static void vTaskWorker(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        vTaskDelay(1000U);
    }
}

static void vTaskBusy(void *pvParameters) {
    volatile uint32_t ulDummy = 0U;
    (void)pvParameters;
    for (;;) {
        ulDummy++;
        if (ulDummy > 100000UL) {
            ulDummy = 0U;
        }
    }
}

static void vTaskMonitor(void *pvParameters) {
    TaskStatus_t xStatus[MAX_TASKS];
    UBaseType_t uxArraySize, uxIndex;
    uint32_t ulTotalRunTime = 0U;
    (void)pvParameters;

    for (;;) {
        uxArraySize = uxTaskGetNumberOfTasks();
        if (uxArraySize > MAX_TASKS) {
            uxArraySize = MAX_TASKS;
        }
        uxArraySize = uxTaskGetSystemState(xStatus, uxArraySize, &ulTotalRunTime);

        DEBUG_INFO(0U, "Task list (%u):\n", (unsigned int)(uxTaskGetNumberOfTasks()));
        for (uxIndex = 0U; uxIndex < uxArraySize; uxIndex++) {
            DEBUG_INFO(0U, "  [%s] prio=%u hwmark=%lu\n", xStatus[uxIndex].pcTaskName,
                       (unsigned int)xStatus[uxIndex].uxCurrentPriority,
                       (unsigned long)xStatus[uxIndex].usStackHighWaterMark);
        }
        vTaskDelay(5000U);
    }
}

void main(void) {
    vGpio_Init();
    (void)xTaskCreate(vTaskWorker, "Worker", TASK_STK_SIZE, NULL, 1U, NULL);
    (void)xTaskCreate(vTaskBusy, "Busy", TASK_STK_SIZE, NULL, 2U, NULL);
    (void)xTaskCreate(vTaskMonitor, "Monitor", TASK_STK_SIZE, NULL, 3U, NULL);
    vTaskStartScheduler();
}