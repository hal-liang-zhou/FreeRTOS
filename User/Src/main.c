#include "main.h"

#define TASK_PRIORITY 1
#define TASK_STK_SIZE 128

static TaskHandle_t xWorkHandle = NULL;

static void vWorkTask(void *parameter) {
    for (;;) {
        DBG_LED = !DBG_LED;
        DEBUG_INFO(0, "Work task is running\n");
        vTaskDelay(1000);
    }
}

static void vCtrlTask(void *parameter) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        vTaskSuspend(xWorkHandle);
        DEBUG_INFO(0, "Work task suspended\n");
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(3000));

        vTaskResume(xWorkHandle);
        DEBUG_INFO(0, "Work task resumed\n");
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(3000));
    }
}

static void vPrintTask(void *parameter) {
    for (;;) {
        UBaseType_t uxMark = uxTaskGetStackHighWaterMark(NULL);
        DEBUG_INFO(0, "Stack remaining: %d words\n", uxMark);
        DEBUG_INFO(0, "Priority: %d\n", uxTaskPriorityGet(NULL));
        vTaskDelay(5000);
    }
}

void main(void) {
#if (DEBUG)
    SEGGER_RTT_Init();
#endif
    vGpio_Init();
    xTaskCreate((TaskFunction_t)vWorkTask, (const char *)"Work", (uint16_t)TASK_STK_SIZE, (void *)NULL, (UBaseType_t)2,
                &xWorkHandle);
    xTaskCreate((TaskFunction_t)vCtrlTask, (const char *)"Ctrl", (uint16_t)TASK_STK_SIZE, (void *)NULL, (UBaseType_t)1,
                NULL);
    xTaskCreate((TaskFunction_t)vPrintTask, (const char *)"Print", (uint16_t)TASK_STK_SIZE, (void *)NULL,
                (UBaseType_t)1, NULL);
    vTaskStartScheduler();
}
