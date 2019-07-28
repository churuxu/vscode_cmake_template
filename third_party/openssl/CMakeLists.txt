cmake_minimum_required(VERSION 3.10)

### cmake template for build package with remote source ### 



set(PACKAGE_NAME openssl)

set(PACKAGE_VERSION 1.1.1c)

set(PACKAGE_URL https://www.openssl.org/source/openssl-${PACKAGE_VERSION}.tar.gz)

set(PACKAGE_TARGET openssl)

#message("package ${PACKAGE_NAME} ${PACKAGE_VERSION}")


#-------------------- package build settings --------------------


set(FLAGS "-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN")
set(CFLAGS ${FLAGS})
set(CXXFLAGS ${FLAGS})

#-------------------- generic --------------------
project(${PACKAGE_NAME})

if(CMAKE_HOST_WIN32)
	set(PACKAGE_CACHE_DIR D:/.xpackage) 
else()
	set(PACKAGE_CACHE_DIR $ENV{HOME}/.xpackage) 
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CACHE_DIR ${PACKAGE_CACHE_DIR}/${PACKAGE_NAME}/${PACKAGE_VERSION})
set(SOURCE_DIR ${CACHE_DIR}/source/openssl-${PACKAGE_VERSION})


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

include_directories(${SOURCE_DIR}/include)
include_directories(${SOURCE_DIR}/crypto/include)
include_directories(${SOURCE_DIR}/crypto)
include_directories(${SOURCE_DIR}/crypto/modes)
include_directories(${SOURCE_DIR}/crypto/ec/curve448)
include_directories(${SOURCE_DIR}/crypto/ec/curve448/arch_32)
include_directories(${SOURCE_DIR})

macro(save_build_info filename)
	set(info_ "")
	set(info_ "${info_}os=${CMAKE_SYSTEM_NAME}\n")
	set(info_ "${info_}cc=${CMAKE_C_COMPILER}\n")
	set(info_ "${info_}cc.version=${CMAKE_C_COMPILER_VERSION}\n")
	set(info_ "${info_}cxx=${CMAKE_CXX_COMPILER}\n")
	set(info_ "${info_}cxx.version=${CMAKE_CXX_COMPILER_VERSION}\n")
	set(info_ "${info_}build_type=${CMAKE_BUILD_TYPE}\n")
	set(info_ "${info_}cflags=${CMAKE_C_FLAGS}\n")
	set(info_ "${info_}cxxflags=${CMAKE_CXX_FLAGS}\n")	
	#set(BUILD_INFO "${info_}")	
	file(WRITE ${filename} "${info_}")
endmacro()

macro(download url name)
	message("download: ${url}")
	file(DOWNLOAD ${url} ${name} STATUS result)
	if(0 IN_LIST result)
		message("download ok")
	else()
		message(FATAL_ERROR "download error")
	endif()
endmacro()

#download source
macro(download_source type url dir name)	
	if(NOT EXISTS ${dir}/source.downloaded)
		if(${type} STREQUAL "git")
			exec("git clone --depth ${url} ${dir}/${name}")
		else()		
			download(${url} ${dir}/${name})
		endif()
		file(TOUCH ${dir}/source.downloaded)
	else()
		#message("source already downloaded")
	endif()
endmacro()

#unpack 
macro(unpack_source type dir name)	
	if(NOT EXISTS ${dir}/source.unpacked)
		if(${type} STREQUAL "git")
			message("git folder need unpack")
			set(result_ 0)
			set(cmd_ 0)
		elseif(${type} STREQUAL "tar.gz")
			file(MAKE_DIRECTORY ${dir}/source)
			set(cmd_ tar -xzvf ${dir}/${name} -C ${dir}/source)			
		elseif(${type} STREQUAL "zip")			
			file(MAKE_DIRECTORY ${dir}/source)
			set(cmd_ unzip -o ${dir}/${name} -d ${dir}/source)
		else()
			message(FATAL_ERROR "not support type ${type}")
		endif()
		if(cmd_)
			message("unpack ${dir}/${name}")
			execute_process(RESULT_VARIABLE result_ COMMAND ${cmd_})
		endif()
		if(0 EQUAL result_)
			message("unpack ok")
		else()		
			message(FATAL_ERROR "unpack error" )
		endif()		
		file(TOUCH ${dir}/source.unpacked)
	else()
		#message("source already unpacked")
	endif()
endmacro()


macro(fetch_source url dir)	
	if(${url} MATCHES ".*.git$")
		set(fetch_type_ "git")
		set(fetch_name_ "source")		
	elseif(${url} MATCHES ".*.tar.gz$")
		set(fetch_type_ "tar.gz")
		set(fetch_name_ "source.tar.gz")		
	else()
		set(fetch_type_ "zip")
		set(fetch_name_ "source.zip")		
	endif()
	download_source(${fetch_type_} ${url} ${dir} ${fetch_name_})
	unpack_source(${fetch_type_} ${dir} ${fetch_name_})
endmacro()


fetch_source(${PACKAGE_URL} ${CACHE_DIR})

file(COPY ${CMAKE_CURRENT_LIST_DIR}/config/ DESTINATION ${SOURCE_DIR})

#-------------------- generic flags --------------------
set(CMAKE_C_FLAGS ${CFLAGS})
set(CMAKE_CXX_FLAGS ${CXXFLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${LDFLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${LDFLAGS})

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(DEFINED LIBS)
	link_libraries(${LIBS})
endif()

#-------------------- glob all files --------------------
#message("source dir: ${SOURCE_DIR}")

file(GLOB_RECURSE CRYPTO_SRCS ${SOURCE_DIR}/crypto/*.c)
file(GLOB_RECURSE SSL_SRCS ${SOURCE_DIR}/ssl/*.c)
file(GLOB_RECURSE ALL_HEADERS ${SOURCE_DIR}/*.h)


#-------------------- exlude files --------------------
list(FILTER CRYPTO_SRCS EXCLUDE REGEX "CMakeFiles")
list(FILTER SSL_SRCS EXCLUDE REGEX "CMakeFiles")
list(FILTER ALL_HEADERS EXCLUDE REGEX "CMakeFiles")
foreach(EXLUDE IN LISTS EXLUDES) 
	list(FILTER CRYPTO_SRCS EXCLUDE REGEX ${EXLUDE})
	list(FILTER SSL_SRCS EXCLUDE REGEX ${EXLUDE})
	list(FILTER ALL_HEADERS EXCLUDE REGEX ${EXLUDE})
endforeach()


#-------------------- includes --------------------
foreach(FILENAME IN LISTS ALL_HEADERS)
	get_filename_component(DIRNAME ${FILENAME} DIRECTORY)	
	if(NOT "${DIRNAME}" IN_LIST INCS)
		list(APPEND INCS ${DIRNAME})		
	endif()
endforeach()
foreach(INC IN LISTS INCS)
	#include_directories(${INC})
endforeach()

#-------------------- get build hash --------------------

set(INFO_FILE ${CMAKE_BINARY_DIR}/build_info.txt) 
save_build_info(${INFO_FILE})
file(MD5 ${INFO_FILE} BUILD_HASH)
#message("build_hash: ${BUILD_HASH}")
set(BUILD_DIR ${CACHE_DIR}/build/${BUILD_HASH})
file(MAKE_DIRECTORY ${BUILD_DIR})
file(COPY ${INFO_FILE} DESTINATION ${BUILD_DIR})


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})


#-------------------- build crypto lib --------------------
set(TARGET_NAME crypto)
set(TARGET_FILE ${BUILD_DIR}/lib${TARGET_NAME}.a)
if(NOT TARGET ${TARGET_NAME})
	if(EXISTS ${TARGET_FILE})
		message("-- Import ${TARGET_FILE}")
		add_library(${TARGET_NAME} STATIC IMPORTED GLOBAL)
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORTED_LOCATION ${TARGET_FILE})
	else()
		message("-- Build ${TARGET_FILE}")
		add_library(${TARGET_NAME} ${CRYPTO_SRCS})
	endif()
endif()

#-------------------- build ssl lib --------------------
set(TARGET_NAME ssl)
set(TARGET_FILE ${BUILD_DIR}/lib${TARGET_NAME}.a)
if(NOT TARGET ${TARGET_NAME})
	if(EXISTS ${TARGET_FILE})
		message("-- Import ${TARGET_FILE}")
		add_library(${TARGET_NAME} STATIC IMPORTED GLOBAL)
		set_target_properties(${TARGET_NAME} PROPERTIES IMPORTED_LOCATION ${TARGET_FILE})
	else()
		message("-- Build ${TARGET_FILE}")
		add_library(${TARGET_NAME} ${SSL_SRCS} )
	endif()
endif()

#-------------------- export info --------------------
set(EXPORT_INFO "")
set(EXPORT_INFO "${EXPORT_INFO}#package ${PACKAGE_NAME}\n")
set(EXPORT_INFO "${EXPORT_INFO}link_directories(${BUILD_DIR})\n")
#set(EXPORT_INFO "${EXPORT_INFO}link_libraries(${PACKAGE_TARGET})\n")
#foreach(DIR IN LISTS INCS)
	set(EXPORT_INFO "${EXPORT_INFO}include_directories(${SOURCE_DIR}/include)\n")
#endforeach()

set(EXPORT_FILE ${CMAKE_BINARY_DIR}/Findssl.cmake)

if(EXISTS ${EXPORT_FILE})
	file(READ ${EXPORT_FILE} OLD_INFO)
endif()
if(NOT "${OLD_INFO}" STREQUAL "${EXPORT_INFO}")
	file(WRITE ${EXPORT_FILE} "${EXPORT_INFO}")
endif()

set(EXPORT_FILE ${CMAKE_BINARY_DIR}/Findcrypto.cmake)

if(EXISTS ${EXPORT_FILE})
	file(READ ${EXPORT_FILE} OLD_INFO)
endif()
if(NOT "${OLD_INFO}" STREQUAL "${EXPORT_INFO}")
	file(WRITE ${EXPORT_FILE} "${EXPORT_INFO}")
endif()
