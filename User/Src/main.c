#include "FreeRTOS.h"
#include "task.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

static volatile uint32_t ulIdleCount = 0U;

void vApplicationIdleHook(void) {
    ulIdleCount++;
    DBG_LED = !DBG_LED;
}

static void vTaskMonitor(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        DEBUG_INFO(0U, "Idle count: %lu\n", (unsigned long)ulIdleCount);
        vTaskDelay(2000U);
    }
}

void main(void) {
    vGpio_Init();
    (void)xTaskCreate(vTaskMonitor, "Monitor", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}