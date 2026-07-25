#pragma once
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "stdbool.h"
#include "stdint.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define DEBUG          1
#define USE_UART_DEBUG 0
#define TIME

#if (DEBUG)
#if (USE_UART_DEBUG)
#ifdef RAW
#define DEBUG_ERROR(ms, x, ...)                                        \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%d][ERROR] " x, __LINE__, ##__VA_ARGS__); \
        }                                                              \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                      \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%d][WARN] " x, __LINE__, ##__VA_ARGS__);  \
        }                                                              \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                         \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%d][INFO] " x, __LINE__, ##__VA_ARGS__);  \
        }                                                              \
    } while (0)
#elif defined(NORMAL)
#define DEBUG_ERROR(ms, x, ...)                                        \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[ERROR] " x, ##__VA_ARGS__);               \
        }                                                              \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                      \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[WARN] " x, ##__VA_ARGS__);                \
        }                                                              \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                         \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[INFO] " x, ##__VA_ARGS__);                \
        }                                                              \
    } while (0)
#elif defined(ALL)
#define DEBUG_ERROR(ms, x, ...)                                                                                  \
    do {                                                                                                         \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                               \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                 \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                           \
            _dbg_last_##__LINE__ = _dbg_now;                                                                     \
            vUart_debug(0, "[\x1B[31m%lu\x1B[0m][\x1B[31m%d\x1B[0m %s][ERROR] " x, _dbg_now, __LINE__, __FILE__, \
                        ##__VA_ARGS__);                                                                          \
        }                                                                                                        \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                                                               \
    do {                                                                                                        \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                              \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                          \
            _dbg_last_##__LINE__ = _dbg_now;                                                                    \
            vUart_debug(0, "[\x1B[33m%lu\x1B[0m][\x1B[33m%d\x1B[0m %s][WARN] " x, _dbg_now, __LINE__, __FILE__, \
                        ##__VA_ARGS__);                                                                         \
        }                                                                                                       \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                                                                  \
    do {                                                                                                        \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                              \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                          \
            _dbg_last_##__LINE__ = _dbg_now;                                                                    \
            vUart_debug(0, "[\x1B[32m%lu\x1B[0m][\x1B[32m%d\x1B[0m %s][INFO] " x, _dbg_now, __LINE__, __FILE__, \
                        ##__VA_ARGS__);                                                                         \
        }                                                                                                       \
    } while (0)
#elif defined(FILE)
#define DEBUG_ERROR(ms, x, ...)                                        \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%s][ERROR] " x, __FILE__, ##__VA_ARGS__); \
        }                                                              \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                      \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%s][WARN] " x, __FILE__, ##__VA_ARGS__);  \
        }                                                              \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                         \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%s][INFO] " x, __FILE__, ##__VA_ARGS__);  \
        }                                                              \
    } while (0)
#elif defined(TIME)
#define DEBUG_ERROR(ms, x, ...)                                         \
    do {                                                                \
        static uint32_t _dbg_last_##__LINE__ = 0U;                      \
        uint32_t _dbg_now = xTaskGetTickCount();                        \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {  \
            _dbg_last_##__LINE__ = _dbg_now;                            \
            vUart_debug(0, "[%lu][ERROR] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                               \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                      \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%lu][WARN] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                              \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                         \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            vUart_debug(0, "[%lu][INFO] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                              \
    } while (0)
#else
#define DEBUG_ERROR(ms, x, ...)
#define DEBUG_WARNING(ms, x, ...)
#define DEBUG_INFO(ms, x, ...)
#endif
#else

#ifdef RAW
#define DEBUG_ERROR(ms, x, ...)                                              \
    do {                                                                     \
        static uint32_t _dbg_last_##__LINE__ = 0U;                           \
        uint32_t _dbg_now = xTaskGetTickCount();                             \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {       \
            _dbg_last_##__LINE__ = _dbg_now;                                 \
            SEGGER_RTT_printf(0, "[%d][ERROR] " x, __LINE__, ##__VA_ARGS__); \
        }                                                                    \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                           \
    do {                                                                    \
        static uint32_t _dbg_last_##__LINE__ = 0U;                          \
        uint32_t _dbg_now = xTaskGetTickCount();                            \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {      \
            _dbg_last_##__LINE__ = _dbg_now;                                \
            SEGGER_RTT_printf(0, "[%d][WARN] " x, __LINE__, ##__VA_ARGS__); \
        }                                                                   \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                              \
    do {                                                                    \
        static uint32_t _dbg_last_##__LINE__ = 0U;                          \
        uint32_t _dbg_now = xTaskGetTickCount();                            \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {      \
            _dbg_last_##__LINE__ = _dbg_now;                                \
            SEGGER_RTT_printf(0, "[%d][INFO] " x, __LINE__, ##__VA_ARGS__); \
        }                                                                   \
    } while (0)
#elif defined(NORMAL)
#define DEBUG_ERROR(ms, x, ...)                                        \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            SEGGER_RTT_printf(0, "[ERROR] " x, ##__VA_ARGS__);         \
        }                                                              \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                      \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            SEGGER_RTT_printf(0, "[WARN] " x, ##__VA_ARGS__);          \
        }                                                              \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                         \
    do {                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                     \
        uint32_t _dbg_now = xTaskGetTickCount();                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) { \
            _dbg_last_##__LINE__ = _dbg_now;                           \
            SEGGER_RTT_printf(0, "[INFO] " x, ##__VA_ARGS__);          \
        }                                                              \
    } while (0)
#elif defined(ALL)
#define DEBUG_ERROR(ms, x, ...)                                                                                        \
    do {                                                                                                               \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                                     \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                       \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                                 \
            _dbg_last_##__LINE__ = _dbg_now;                                                                           \
            SEGGER_RTT_printf(0, "[\x1B[31m%lu\x1B[0m][\x1B[31m%d\x1B[0m %s][ERROR] " x, _dbg_now, __LINE__, __FILE__, \
                              ##__VA_ARGS__);                                                                          \
        }                                                                                                              \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                                                                     \
    do {                                                                                                              \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                                    \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                      \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                                \
            _dbg_last_##__LINE__ = _dbg_now;                                                                          \
            SEGGER_RTT_printf(0, "[\x1B[33m%lu\x1B[0m][\x1B[33m%d\x1B[0m %s][WARN] " x, _dbg_now, __LINE__, __FILE__, \
                              ##__VA_ARGS__);                                                                         \
        }                                                                                                             \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                                                                        \
    do {                                                                                                              \
        static uint32_t _dbg_last_##__LINE__ = 0U;                                                                    \
        uint32_t _dbg_now = xTaskGetTickCount();                                                                      \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {                                                \
            _dbg_last_##__LINE__ = _dbg_now;                                                                          \
            SEGGER_RTT_printf(0, "[\x1B[32m%lu\x1B[0m][\x1B[32m%d\x1B[0m %s][INFO] " x, _dbg_now, __LINE__, __FILE__, \
                              ##__VA_ARGS__);                                                                         \
        }                                                                                                             \
    } while (0)
#elif defined(FILE)
#define DEBUG_ERROR(ms, x, ...)                                              \
    do {                                                                     \
        static uint32_t _dbg_last_##__LINE__ = 0U;                           \
        uint32_t _dbg_now = xTaskGetTickCount();                             \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {       \
            _dbg_last_##__LINE__ = _dbg_now;                                 \
            SEGGER_RTT_printf(0, "[%s][ERROR] " x, __FILE__, ##__VA_ARGS__); \
        }                                                                    \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                           \
    do {                                                                    \
        static uint32_t _dbg_last_##__LINE__ = 0U;                          \
        uint32_t _dbg_now = xTaskGetTickCount();                            \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {      \
            _dbg_last_##__LINE__ = _dbg_now;                                \
            SEGGER_RTT_printf(0, "[%s][WARN] " x, __FILE__, ##__VA_ARGS__); \
        }                                                                   \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                              \
    do {                                                                    \
        static uint32_t _dbg_last_##__LINE__ = 0U;                          \
        uint32_t _dbg_now = xTaskGetTickCount();                            \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {      \
            _dbg_last_##__LINE__ = _dbg_now;                                \
            SEGGER_RTT_printf(0, "[%s][INFO] " x, __FILE__, ##__VA_ARGS__); \
        }                                                                   \
    } while (0)
#elif defined(TIME)
#define DEBUG_ERROR(ms, x, ...)                                               \
    do {                                                                      \
        static uint32_t _dbg_last_##__LINE__ = 0U;                            \
        uint32_t _dbg_now = xTaskGetTickCount();                              \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {        \
            _dbg_last_##__LINE__ = _dbg_now;                                  \
            SEGGER_RTT_printf(0, "[%lu][ERROR] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                                     \
    } while (0)
#define DEBUG_WARNING(ms, x, ...)                                            \
    do {                                                                     \
        static uint32_t _dbg_last_##__LINE__ = 0U;                           \
        uint32_t _dbg_now = xTaskGetTickCount();                             \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {       \
            _dbg_last_##__LINE__ = _dbg_now;                                 \
            SEGGER_RTT_printf(0, "[%lu][WARN] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                                    \
    } while (0)
#define DEBUG_INFO(ms, x, ...)                                               \
    do {                                                                     \
        static uint32_t _dbg_last_##__LINE__ = 0U;                           \
        uint32_t _dbg_now = xTaskGetTickCount();                             \
        if ((ms) == 0U || (_dbg_now - _dbg_last_##__LINE__) >= (ms)) {       \
            _dbg_last_##__LINE__ = _dbg_now;                                 \
            SEGGER_RTT_printf(0, "[%lu][INFO] " x, _dbg_now, ##__VA_ARGS__); \
        }                                                                    \
    } while (0)
#else
#define DEBUG_ERROR(ms, x, ...)
#define DEBUG_WARNING(ms, x, ...)
#define DEBUG_INFO(ms, x, ...)
#endif
#endif
#else
#define DEBUG_ERROR(ms, x, ...)
#define DEBUG_WARNING(ms, x, ...)
#define DEBUG_INFO(ms, x, ...)
#endif

#define TEST(x, y)                      \
    if (!(x)) {                         \
        DEBUG_ERROR(0, y " failed.\n"); \
        return;                         \
    }                                   \
    DEBUG_INFO(0, y " succeed.\n");

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif
