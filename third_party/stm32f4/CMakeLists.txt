cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)


set(PACKAGE_NAME stm32f4)

set(PACKAGE_VERSION 170)

set(SRC_DIR STM32Cube_FW_F4_V1.7.0)

set(PACKAGE_URL http://sw-center.st.com/packs/resource/firmware/stm32cube_fw_f4_v${PACKAGE_VERSION}.zip)

set(EXCLUDES ".*_ll_.*" ".*Legacy/.*")

if(NOT ${PACKAGE_NAME} IN_LIST LIBS)
	return()
endif()



if(${CPU_NAME} MATCHES "STM32F405..")
	set(MCU_DEFINE STM32F405xx)
elseif(${CPU_NAME} MATCHES "STM32F415..")
	set(MCU_DEFINE STM32F415xx)
elseif(${CPU_NAME} MATCHES "STM32F407..")
	set(MCU_DEFINE STM32F407xx)
elseif(${CPU_NAME} MATCHES "STM32F417..")
	set(MCU_DEFINE STM32F417xx)
elseif(${CPU_NAME} MATCHES "STM32F427..")
	set(MCU_DEFINE STM32F427xx)
elseif(${CPU_NAME} MATCHES "STM32F437..")
	set(MCU_DEFINE STM32F437xx)
elseif(${CPU_NAME} MATCHES "STM32F429..")
	set(MCU_DEFINE STM32F429xx)
elseif(${CPU_NAME} MATCHES "STM32F439..")
	set(MCU_DEFINE STM32F439xx)
elseif(${CPU_NAME} MATCHES "STM32F401.[BC]")
	set(MCU_DEFINE STM32F401xC)
elseif(${CPU_NAME} MATCHES "STM32F401.[DE]")
	set(MCU_DEFINE STM32F401xE)
elseif(${CPU_NAME} MATCHES "STM32F411.[DE]")
	set(MCU_DEFINE STM32F411xE)
elseif(${CPU_NAME} MATCHES "STM32F446..")
	set(MCU_DEFINE STM32F446xx)
else()
	message(FATAL_ERROR "Must set valid 'MCU' variable")
endif()

string(TOLOWER ${MCU_DEFINE} mcu_define_lcase)

set(FLAGS "-DUSE_HAL_DRIVER -D${MCU_DEFINE}")



xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(HAL_DRIVER_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/STM32F4xx_HAL_Driver)
set(CMSIS_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/CMSIS)
set(CMSIS_ST_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx)


if(NOT EXISTS ${HAL_DRIVER_DIR}/Inc/stm32f4xx_hal_conf.h)
	configure_file(${HAL_DRIVER_DIR}/Inc/stm32f4xx_hal_conf_template.h ${HAL_DRIVER_DIR}/Inc/stm32f4xx_hal_conf.h COPYONLY)
endif()

set(EXPORT_INCS ${HAL_DRIVER_DIR}/Inc ${CMSIS_ST_DIR}/Include ${CMSIS_DIR}/Include)

include_directories(${EXPORT_INCS})

xpackage_glob_src(SRCS ${HAL_DRIVER_DIR}/Src/*.c  EXCLUDES)

list(APPEND SRCS ${CMSIS_ST_DIR}/Source/Templates/system_stm32f4xx.c)
list(APPEND SRCS ${CMSIS_ST_DIR}/Source/Templates/gcc/startup_${mcu_define_lcase}.s)

xpackage_build_target(${PACKAGE_NAME} ${BUILD_ROOT} SRCS EXPORT_INCS "")



