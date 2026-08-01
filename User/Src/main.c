#include "FreeRTOS.h"
#include "task.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U
#define CREATOR_LOOP  3U

static void vTaskWorker(void *pvParameters) {
    (void)pvParameters;
    DEBUG_INFO(0U, "Worker: start\n");
    DEBUG_INFO(0U, "Free heap: %lu\n", (unsigned long)xPortGetFreeHeapSize());
    vTaskDelay(500U);
    DEBUG_INFO(0U, "Worker: end, delete self\n");
    vTaskDelete(NULL);
}

static void vTaskCreator(void *pvParameters) {
    (void)pvParameters;
    uint32_t ulIndex;
    for (ulIndex = 0U; ulIndex < CREATOR_LOOP; ulIndex++) {
        DEBUG_INFO(0U, "Creator: create worker %lu\n", ulIndex);
        (void)xTaskCreate(vTaskWorker, "Worker", TASK_STK_SIZE, NULL, 1U, NULL);
        vTaskDelay(1000U);
        DEBUG_INFO(0U, "Free heap now: %lu, min ever: %lu\n",
                   (unsigned long)xPortGetFreeHeapSize(),
                   (unsigned long)xPortGetMinimumEverFreeHeapSize());
    }
    vTaskDelete(NULL);
}

void main(void) {
    vGpio_Init();
    (void)xTaskCreate(vTaskCreator, "Creator", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}
