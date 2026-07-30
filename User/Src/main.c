#include "FreeRTOS.h"
#include "task.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

static TaskHandle_t xWorkerHandle = NULL;

static void vNotifier(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        vTaskDelay(2000U);
        (void)xTaskNotifyGive(xWorkerHandle);
        DEBUG_INFO(0U, "Notifier: give\n");
    }
}

static void vWorker(void *pvParameters) {
    (void)pvParameters;
    uint32_t ulNotified;
    for (;;) {
        ulNotified = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (ulNotified > 0U) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "Worker: take, count=%lu\n", ulNotified);
        }
    }
}

void main(void) {
    vGpio_Init();
    (void)xTaskCreate(vWorker, "Worker", TASK_STK_SIZE, NULL, 1U, &xWorkerHandle);
    (void)xTaskCreate(vNotifier, "Notifier", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}