cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)



set(PACKAGE_NAME duktape)

set(PACKAGE_VERSION 2.3.0)

set(PACKAGE_URL https://github.com/svaarala/duktape-releases/archive/v${PACKAGE_VERSION}.tar.gz)

set(DIR_NAME ${PACKAGE_NAME}-releases-${PACKAGE_VERSION})

set(FLAGS )

set(EXLUDES ".*bindings/.*" ".*tests/.*")



xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(SOURCE_DIR "${SRC_ROOT}/${DIR_NAME}/src-noline")
set(EXPORT_INCS "${SOURCE_DIR}")

xpackage_glob_src(SRCS ${SOURCE_DIR}/*.c  EXLUDES)

xpackage_build_target(${PACKAGE_NAME} ${BUILD_ROOT} SRCS EXPORT_INCS "")

