#include "FreeRTOS.h"
#include "task.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

static volatile uint32_t ulTicklessWakeups = 0U;

static void vTaskTicker(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        DEBUG_INFO(0U, "Tick: %lu, wakeups: %lu\n",
                   (unsigned long)xTaskGetTickCount(),
                   (unsigned long)ulTicklessWakeups);
        vTaskDelay(3000U);
    }
}

static void vTaskPulse(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        vTaskDelay(100U);
        DBG_LED = !DBG_LED;
        ulTicklessWakeups++;
    }
}

void main(void) {
    vGpio_Init();
    (void)xTaskCreate(vTaskTicker, "Ticker", TASK_STK_SIZE, NULL, 1U, NULL);
    (void)xTaskCreate(vTaskPulse, "Pulse", TASK_STK_SIZE, NULL, 2U, NULL);
    vTaskStartScheduler();
}