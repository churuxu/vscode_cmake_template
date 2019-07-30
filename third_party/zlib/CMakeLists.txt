cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)


set(PACKAGE_NAME zlib)

set(PACKAGE_VERSION 1.2.11)

set(PACKAGE_URL https://downloads.sourceforge.net/project/libpng/zlib/${PACKAGE_VERSION}/zlib-${PACKAGE_VERSION}.tar.gz)

set(SRC_DIR zlib-${PACKAGE_VERSION})

set(FLAGS )

set(EXLUDES ".*contrib/.*" ".*examples/.*" ".*test/.*")


#include_directories(...)

xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(EXPORT_INCS "${SRC_ROOT}/${SRC_DIR}")

xpackage_glob_src(SRCS ${SRC_ROOT}/${SRC_DIR}/*.c  EXLUDES)

xpackage_build_target(z ${BUILD_ROOT} SRCS EXPORT_INCS "")



