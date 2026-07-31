#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "debug_config.h"
#include "gpiox.h"
#include "gpiodef.h"

#define TASK_STK_SIZE 128U

#define TIMER_ONE_SHOT     (pdFALSE)
#define TIMER_AUTO_RELOAD  (pdTRUE)

static TimerHandle_t xTimerOneShot = NULL;
static TimerHandle_t xTimerAutoReload = NULL;

static void vTimerOneShotCallback(TimerHandle_t xTimer) {
    (void)xTimer;
    DEBUG_INFO(0U, "One-shot timer fired\n");
}

static void vTimerAutoReloadCallback(TimerHandle_t xTimer) {
    (void)xTimer;
    DBG_LED = !DBG_LED;
    DEBUG_INFO(0U, "Auto-reload timer fired\n");
}

static void vTaskMonitor(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        if (xTimerIsTimerActive(xTimerOneShot) == pdTRUE) {
            DEBUG_INFO(0U, "One-shot timer: running\n");
        } else {
            DEBUG_INFO(0U, "One-shot timer: stopped\n");
        }
        vTaskDelay(3000U);
    }
}

void main(void) {
    vGpio_Init();
    xTimerOneShot = xTimerCreate("OneShot", pdMS_TO_TICKS(3000U), TIMER_ONE_SHOT, NULL, vTimerOneShotCallback);
    xTimerAutoReload = xTimerCreate("AutoReload", pdMS_TO_TICKS(1000U), TIMER_AUTO_RELOAD, NULL, vTimerAutoReloadCallback);
    if ((xTimerOneShot != NULL) && (xTimerAutoReload != NULL)) {
        (void)xTimerStart(xTimerOneShot, 0U);
        (void)xTimerStart(xTimerAutoReload, 0U);
    }
    (void)xTaskCreate(vTaskMonitor, "Monitor", TASK_STK_SIZE, NULL, 1U, NULL);
    vTaskStartScheduler();
}