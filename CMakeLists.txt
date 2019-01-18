cmake_minimum_required(VERSION 3.7)

#-------------------- set folder name as project name -------------------- 
file(RELATIVE_PATH PROJNAME ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})
project(${PROJNAME})

#-------------------- glob all .c .cpp file as source --------------------
file(GLOB_RECURSE ALL_SRCS *.cpp *.c *.cc)
list(FILTER ALL_SRCS EXCLUDE REGEX CMake)
foreach(SRC IN LISTS ALL_SRCS)
	get_filename_component(FILENAME ${SRC} NAME)
	if(${FILENAME} MATCHES ".*test.*")
		list(APPEND TEST_SRCS ${SRC})
	else()
		list(APPEND APP_SRCS ${SRC})
	endif()
endforeach()

#------------------ glob all folder contains .h file as include folder -----------------
file(GLOB_RECURSE HEADERS *.h)
list(FILTER HEADERS EXCLUDE REGEX CMake)
list(APPEND INCS ${CMAKE_CURRENT_LIST_DIR})
foreach(FILENAME IN LISTS HEADERS)
	get_filename_component(DIRNAME ${FILENAME} DIRECTORY)	
	if(NOT "${DIRNAME}" IN_LIST INCS)
		list(APPEND INCS ${DIRNAME})		
	endif()
endforeach()
foreach(INC IN LISTS INCS)
	include_directories(${INC})
endforeach()


#-------------------- commom build flags --------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
else()
	add_compile_options(-g -Wall)	
	if(${CMAKE_HOST_UNIX})
		link_libraries(-ldl -lpthread)
	elseif(${CMAKE_HOST_WIN32})
		link_libraries(-lpsapi -lws2_32)
	endif()	
endif()

add_executable(${PROJNAME} ${APP_SRCS})

#-------------------- build test application --------------------
list(LENGTH TEST_SRCS TEST_SRCS_COUNT)
if(NOT ${TEST_SRCS_COUNT} EQUAL 0)		
	add_executable(${PROJNAME}-test ${TEST_SRCS})
	target_compile_definitions(${PROJNAME}-test PUBLIC UNITTEST)	
endif()


