cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)



set(PACKAGE_NAME json-parser)

set(PACKAGE_VERSION 1.1.0)

set(PACKAGE_URL https://github.com/udp/json-parser/archive/v${PACKAGE_VERSION}.zip)

set(DIR_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})

set(FLAGS )

set(EXLUDES ".*bindings/.*" ".*tests/.*")



xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(EXPORT_INCS "${SRC_ROOT}/${DIR_NAME}")

xpackage_glob_src(SRCS ${SRC_ROOT}/${DIR_NAME}/*.c  EXLUDES)

xpackage_build_target(${PACKAGE_NAME} ${BUILD_ROOT} SRCS EXPORT_INCS "")

