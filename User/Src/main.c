#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128

static SemaphoreHandle_t xBinarySemaphore = NULL;

static void vTask1(void *parameter) {
    for (;;) {
        xSemaphoreGive(xBinarySemaphore);
        DEBUG_INFO(0, "Task1: Semaphore given\n");
        vTaskDelay(2000);
    }
}

static void vTask2(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0, "Task2: Semaphore taken\n");
        }
    }
}

void main(void) {
    vGpio_Init();
    xBinarySemaphore = xSemaphoreCreateBinary();
    xTaskCreate(vTask1, "Task1", TASK_STK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", TASK_STK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}