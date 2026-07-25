#include "main.h"

#define TASK_STK_SIZE 128

static QueueHandle_t xQueue = NULL;

static void vSenderTask(void *parameter) {
    uint32_t ulValue = 0;
    for (;;) {
        xQueueSend(xQueue, &ulValue, portMAX_DELAY);
        DEBUG_INFO(0, "Sent: %lu\n", ulValue);
        ulValue++;
        vTaskDelay(1000);
    }
}

static void vReceiverTask(void *parameter) {
    uint32_t ulRecv;
    for (;;) {
        if (xQueueReceive(xQueue, &ulRecv, portMAX_DELAY) == pdPASS) {
            DBG_LED = !DBG_LED;
            DEBUG_INFO(0, "Received: %lu\n", ulRecv);
        }
    }
}

void main(void) {
#if (DEBUG)
    SEGGER_RTT_Init();
#endif
    vGpio_Init();
    xQueue = xQueueCreate(4, sizeof(uint32_t));
    xTaskCreate((TaskFunction_t)vSenderTask, (const char *)"Sender", (uint16_t)TASK_STK_SIZE, (void *)NULL,
                (UBaseType_t)1, NULL);
    xTaskCreate((TaskFunction_t)vReceiverTask, (const char *)"Receiver", (uint16_t)TASK_STK_SIZE, (void *)NULL,
                (UBaseType_t)1, NULL);
    vTaskStartScheduler();
}