cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)



set(PACKAGE_NAME curl)

set(PACKAGE_VERSION 7.65.1)

set(PACKAGE_URL https://curl.haxx.se/download/curl-${PACKAGE_VERSION}.tar.gz)

set(DIR_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})

set(FLAGS "-DCURL_STATICLIB -DBUILDING_LIBCURL -DUSE_OPENSSL -DENABLE_IPV6 -DCURL_DISABLE_LDAP -DHAVE_GETADDRINFO")

set(EXLUDES )


# add openssl depends
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../openssl ${CMAKE_BINARY_DIR}/openssl)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
find_package(ssl)
find_package(crypto)



xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(SOURCE_DIR "${SRC_ROOT}/${DIR_NAME}")
set(EXPORT_INCS "${SOURCE_DIR}/include")

set(EXPORT_DEFS "-DCURL_STATICLIB")

include_directories("${SOURCE_DIR}/lib")


xpackage_glob_src(SRCS ${SOURCE_DIR}/lib/*.c  EXLUDES)

xpackage_build_target(${PACKAGE_NAME} ${BUILD_ROOT} SRCS EXPORT_INCS EXPORT_DEFS)



