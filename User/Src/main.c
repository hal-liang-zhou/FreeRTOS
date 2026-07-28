#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

static SemaphoreHandle_t xMutex = NULL;
static uint32_t ulSharedCounter = 0U;

static void vTaskWriter(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            ulSharedCounter++;
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "Writer: count=%lu\n", ulSharedCounter);
            (void)xSemaphoreGive(xMutex);
        }
        vTaskDelay(2000U);
    }
}

static void vTaskReader(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            DEBUG_INFO(0U, "Reader: count=%lu\n", ulSharedCounter);
            (void)xSemaphoreGive(xMutex);
        }
        vTaskDelay(1000U);
    }
}

void main(void) {
    vGpio_Init();
    xMutex = xSemaphoreCreateMutex();
    (void)xTaskCreate(vTaskWriter, "Writer", TASK_STK_SIZE, NULL, 1U, NULL);
    (void)xTaskCreate(vTaskReader, "Reader", TASK_STK_SIZE, NULL, 2U, NULL);
    vTaskStartScheduler();
}