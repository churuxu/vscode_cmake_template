cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)


set(PACKAGE_NAME stm32f1)

set(PACKAGE_VERSION 170)

set(SRC_DIR STM32Cube_FW_F1_V1.7.0)

set(PACKAGE_URL http://sw-center.st.com/packs/resource/firmware/stm32cube_fw_f1_v${PACKAGE_VERSION}.zip)

set(EXCLUDES ".*_ll_.*" ".*Legacy/.*")

if(NOT ${PACKAGE_NAME} IN_LIST LIBS)
	return()
endif()


#100
if(${CPU_NAME} MATCHES "STM32F100.[48B]")
	set(MCU_DEFINE STM32F100xB)
elseif(${CPU_NAME} MATCHES "STM32F100.[CDE]")
	set(MCU_DEFINE STM32F100xE)
#101
elseif(${CPU_NAME} MATCHES "STM32F101.[46]")
	set(MCU_DEFINE STM32F101x6)
elseif(${CPU_NAME} MATCHES "STM32F101.[8B]")
	set(MCU_DEFINE STM32F101xB)
elseif(${CPU_NAME} MATCHES "STM32F101.[CDE]")
	set(MCU_DEFINE STM32F101xE)
elseif(${CPU_NAME} MATCHES "STM32F101.[FG]")
	set(MCU_DEFINE STM32F101xG)
#102
elseif(${CPU_NAME} MATCHES "STM32F102.[46]")
	set(MCU_DEFINE STM32F102x6)
elseif(${CPU_NAME} MATCHES "STM32F102.[8B]")
	set(MCU_DEFINE STM32F102xB)
#103
elseif(${CPU_NAME} MATCHES "STM32F103.[46]")
	set(MCU_DEFINE STM32F103x6)
elseif(${CPU_NAME} MATCHES "STM32F103.[8B]")
	set(MCU_DEFINE STM32F103xB)
elseif(${CPU_NAME} MATCHES "STM32F103.[CDE]")
	set(MCU_DEFINE STM32F103xE)
elseif(${CPU_NAME} MATCHES "STM32F103.[FG]")
	set(MCU_DEFINE STM32F103xG)
#105
elseif(${CPU_NAME} MATCHES "STM32F105..")
	set(MCU_DEFINE STM32F105xC)
#107
elseif(${CPU_NAME} MATCHES "STM32F107..")
	set(MCU_DEFINE STM32F107xC)
else()
	message(FATAL_ERROR "Must set valid 'MCU' variable")
endif()

string(TOLOWER ${MCU_DEFINE} mcu_define_lcase)

set(FLAGS "-DUSE_HAL_DRIVER -D${MCU_DEFINE}")



xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(HAL_DRIVER_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/STM32F1xx_HAL_Driver)
set(CMSIS_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/CMSIS)
set(CMSIS_ST_DIR ${SRC_ROOT}/${SRC_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx)


if(NOT EXISTS ${HAL_DRIVER_DIR}/Inc/stm32f1xx_hal_conf.h)
	configure_file(${HAL_DRIVER_DIR}/Inc/stm32f1xx_hal_conf_template.h ${HAL_DRIVER_DIR}/Inc/stm32f1xx_hal_conf.h COPYONLY)
endif()

set(EXPORT_INCS ${HAL_DRIVER_DIR}/Inc ${CMSIS_ST_DIR}/Include ${CMSIS_DIR}/Include)

include_directories(${EXPORT_INCS})

xpackage_glob_src(SRCS ${HAL_DRIVER_DIR}/Src/*.c  EXCLUDES)

list(APPEND SRCS ${CMSIS_ST_DIR}/Source/Templates/system_stm32f1xx.c)
list(APPEND SRCS ${CMSIS_ST_DIR}/Source/Templates/gcc/startup_${mcu_define_lcase}.s)

xpackage_build_target(${PACKAGE_NAME} ${BUILD_ROOT} SRCS EXPORT_INCS FLAGS)



