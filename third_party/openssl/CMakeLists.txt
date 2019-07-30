cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../xpackage.cmake)


set(PACKAGE_NAME openssl)

set(PACKAGE_VERSION 1.1.1c)

set(PACKAGE_URL https://www.openssl.org/source/openssl-${PACKAGE_VERSION}.tar.gz)

set(SRC_DIR openssl-${PACKAGE_VERSION})

set(FLAGS "-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN")

list(APPEND EXLUDES ".*asm/.*")
list(APPEND EXLUDES ".*LPdir.*")
list(APPEND EXLUDES ".*ppccap.c")
list(APPEND EXLUDES ".*armcap.c")
list(APPEND EXLUDES ".*s390xcap.c")
list(APPEND EXLUDES ".*sparcv9cap.c")
list(APPEND EXLUDES ".*ecp_nistz256_table.c") 
list(APPEND EXLUDES ".*eng_devcrypto.c") 
list(APPEND EXLUDES ".*poly1305_base2_44.c") 
list(APPEND EXLUDES ".*poly1305_ieee754.c") 
list(APPEND EXLUDES ".*aes_x86core.c")


xpackage_init(${PACKAGE_NAME} ${PACKAGE_VERSION} ${PACKAGE_URL} "${FLAGS}" SRC_ROOT BUILD_ROOT)

set(SOURCE_DIR "${SRC_ROOT}/${SRC_DIR}")
set(EXPORT_INCS "${SOURCE_DIR}/include")

#config source
file(COPY ${CMAKE_CURRENT_LIST_DIR}/config/ DESTINATION ${SOURCE_DIR})

include_directories(${SOURCE_DIR}/include)
include_directories(${SOURCE_DIR}/crypto/include)
include_directories(${SOURCE_DIR}/crypto)
include_directories(${SOURCE_DIR}/crypto/modes)
include_directories(${SOURCE_DIR}/crypto/ec/curve448)
include_directories(${SOURCE_DIR}/crypto/ec/curve448/arch_32)
include_directories(${SOURCE_DIR})

# target crypto
xpackage_glob_src(CRYPTO_SRCS ${SOURCE_DIR}/crypto/*.c  EXLUDES)

xpackage_build_target(crypto ${BUILD_ROOT} CRYPTO_SRCS EXPORT_INCS "")

# target ssl
xpackage_glob_src(SSL_SRCS ${SOURCE_DIR}/ssl/*.c  EXLUDES)

xpackage_build_target(ssl ${BUILD_ROOT} SSL_SRCS EXPORT_INCS "")


