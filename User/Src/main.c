#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128

static SemaphoreHandle_t xCountingSemaphore = NULL;

static void vProducer(void *parameter) {
    for (;;) {
        xSemaphoreGive(xCountingSemaphore);
        DEBUG_INFO(0, "Produce: +1\n");
        xSemaphoreGive(xCountingSemaphore);
        DEBUG_INFO(0, "Produce: +1\n");
        vTaskDelay(3000);
    }
}

static void vConsumer(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(xCountingSemaphore, 2000) == pdTRUE) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0, "Consume: -1\n");
            vTaskDelay(500);
        } else {
            DEBUG_INFO(0, "No resource\n");
        }
    }
}

void main(void) {
    vGpio_Init();
    xCountingSemaphore = xSemaphoreCreateCounting(5, 0);
    xTaskCreate(vProducer, "Producer", TASK_STK_SIZE, NULL, 1, NULL);
    xTaskCreate(vConsumer, "Consumer", TASK_STK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}