
set(CPU_TYPE cortex-m3)

set(CPU_NAME STM32F103C8)

set(LDSCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32F103XB_FLASH.ld)


list(APPEND LIBS stm32f1)

list(APPEND DEFINES STM32)

include(${CMAKE_CURRENT_LIST_DIR}/../stm32_config.cmake)

