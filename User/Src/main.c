#include "main.h"

#define TASK_PRIORITY 1
#define TASK_STK_SIZE 128

static void vTask(void *parameter) {
    for (;;) {
        DBG_LED = !DBG_LED;
        DEBUG_INFO(0, "Task is running...\n");
        vTaskDelay(1000);
    }
}

void main(void) {
#if (DEBUG)
    SEGGER_RTT_Init();
#endif
    vGpio_Init();
    xTaskCreate((TaskFunction_t)vTask, (const char *)"Task", (uint16_t)TASK_STK_SIZE, (void *)NULL,
                (UBaseType_t)TASK_PRIORITY, NULL);
    vTaskStartScheduler();
}
