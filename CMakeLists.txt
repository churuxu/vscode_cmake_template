cmake_minimum_required(VERSION 3.7)

#-------------------- set folder name as project name -------------------- 
file(RELATIVE_PATH PROJNAME ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})
project(${PROJNAME})

#-------------------- glob all .c .cpp file as source --------------------
file(GLOB_RECURSE SRCS *.cpp *.c)
list(FILTER SRCS EXCLUDE REGEX CMake)

#------------------ glob all folder named inlude as include path -----------------
file(GLOB_RECURSE INCS LIST_DIRECTORIES true include)
foreach(NAME IN LISTS INCS)
if(${NAME} MATCHES ".+include$")
include_directories(${NAME})
endif()
endforeach()

#-------------------- commom build flags --------------------
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

add_executable(${PROJNAME} ${SRCS})