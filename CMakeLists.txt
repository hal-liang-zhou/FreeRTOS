cmake_minimum_required(VERSION 4.2.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(pro LANGUAGES C ASM)

set(TARGET task_notification)
set(DEBUG 0)
set(OPT "-O0")

set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb")
set(COMMON_FLAGS "${MCU_FLAGS} -specs=nano.specs")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${OPT} -MMD -MP -MF\"(@:%.o=%.d)\" -Werror")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} ${OPT} -MMD -MP -MF\"(@:%.o=%.d)\"")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -gdwarf -ggdb")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG")
endif()

file(GLOB_RECURSE C_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/User/Src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Mcal/Src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/STM32F10x_StdPeriph_Driver/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/CMSIS/CM3/CoreSupport/core_cm3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Debug/Src/SEGGER_RTT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Debug/Src/SEGGER_RTT_printf.c
)

list(FILTER C_SOURCES EXCLUDE REGEX ".*portable.*")
list(FILTER C_SOURCES EXCLUDE REGEX ".*build.*")
list(APPEND C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os/portable/MemMang/heap_4.c")
list(APPEND C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os/portable/GCC/ARM_CM3/port.c")
list(FILTER C_SOURCES EXCLUDE REGEX "SEGGER_RTT_Syscalls_GCC.c")

set(ASM_SOURCES "./Lib/StartUp/startup_stm32f10x_hd.s")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/User/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Cdd/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Interface/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Mcal/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/CMSIS/CM3/CoreSupport
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/CMSIS/CM3/DeviceSupport
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/STM32F10x_StdPeriph_Driver/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Os/portable/GCC/ARM_CM3
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Std/CMSIS/CM3/DeviceSupport/ST/STM32F10x
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Debug/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras/Fs/source
    ${CMAKE_CURRENT_SOURCE_DIR}/Infras
)

set(LDSCRIPT "${CMAKE_SOURCE_DIR}/Lib/Link/stm32_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T${LDSCRIPT} -Wl,-Map=${TARGET}.map,--cref,--print-memory-usage,--gc-sections,-z,noexecstack,--no-warn-rwx-segments")

add_executable(${TARGET} ${C_SOURCES} ${ASM_SOURCES})
target_link_libraries(${TARGET} -lc -lm -lnosys)

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET}> ${TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} -O srec $<TARGET_FILE:${TARGET}> ${TARGET}.srec
)
