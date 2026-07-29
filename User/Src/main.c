#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

#define BIT_A  (1UL << 0UL)
#define BIT_B  (1UL << 1UL)

static EventGroupHandle_t xEventGroup = NULL;

static void vTaskA(void *pvParameters) {
    (void)pvParameters;
    EventBits_t uxBits;
    for (;;) {
        uxBits = xEventGroupWaitBits(xEventGroup, BIT_A, pdTRUE, pdFALSE, portMAX_DELAY);
        if ((uxBits & BIT_A) != 0U) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0U, "TaskA: event A received\n");
        }
    }
}

static void vTaskB(void *pvParameters) {
    (void)pvParameters;
    EventBits_t uxBits;
    for (;;) {
        uxBits = xEventGroupWaitBits(xEventGroup, BIT_B, pdTRUE, pdFALSE, portMAX_DELAY);
        if ((uxBits & BIT_B) != 0U) {
            DEBUG_INFO(0U, "TaskB: event B received\n");
        }
    }
}

static void vTaskControl(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        vTaskDelay(2000U);
        (void)xEventGroupSetBits(xEventGroup, BIT_A);
        vTaskDelay(1000U);
        (void)xEventGroupSetBits(xEventGroup, BIT_B);
    }
}

void main(void) {
    vGpio_Init();
    xEventGroup = xEventGroupCreate();
    (void)xTaskCreate(vTaskControl, "Control", TASK_STK_SIZE, NULL, 2U, NULL);
    (void)xTaskCreate(vTaskA, "TaskA", TASK_STK_SIZE, NULL, 1U, NULL);
    (void)xTaskCreate(vTaskB, "TaskB", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}