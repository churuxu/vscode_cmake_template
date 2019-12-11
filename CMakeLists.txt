cmake_minimum_required(VERSION 3.7)

# CC CXX CFLAGS CXXFLAGS LDFLAGS

#--------------------- generic settings ----------------------

file(RELATIVE_PATH PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})
#set(PROJECT_NAME "proj")

set(SRC_EXTS *.cpp *.c *.cc)

set(HEADER_EXTS *.h *.hpp)

list(APPEND EXCLUDES ".*/config/.*")

set(FLAGS )

set(LDFLAGS "")


set(LIBS json-parser)


#for windows gui application  (1=gui 0=console)
#set(NO_CONSOLE 1)

#main source file name (regex)
#set(MAIN_FILTER "^main\..*")

#test source file name (regex)
#set(TEST_FILTER ".*test.*\..*")


#----------------------- external config ----------------------
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/config/selected.txt")
	file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/config/selected.txt" CONFIG_SEL LIMIT_COUNT 1)
	set(CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/config/${CONFIG_SEL})
	message("-- Configuring with ${CONFIG_SEL}")
endif()
include(${CONFIG_DIR}/config.cmake OPTIONAL)



#-------------------- default variable --------------------
if(NOT DEFINED NO_CONSOLE)
	set(NO_CONSOLE 0)
endif()
if(NOT DEFINED MAIN_FILTER)
	set(MAIN_FILTER "^main\..*")
endif()
if(NOT DEFINED TEST_FILTER)
	set(TEST_FILTER ".*test.*\..*")
endif()
if(NOT DEFINED BUILD_TEST)
	set(BUILD_TEST 0)
endif()
set(CFLAGS ${FLAGS})
set(CXXFLAGS ${FLAGS})

project(${PROJECT_NAME})

#-------------------- platform speical --------------------
if(${WIN32})
	file(GLOB_RECURSE RC_SRCS "*.rc")
	add_compile_options(-DUNICODE -D_UNICODE)
	link_libraries(comctl32 psapi ws2_32)
	if(${MINGW})
		if(${NO_CONSOLE})
			set(LDFLAGS "${LDFLAGS} -Wl,--subsystem,windows")
		endif()
	endif()
endif()


#-------------------- compiler speical --------------------

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC) #vc
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
else() #gcc
	add_compile_options(-Wall)
endif()


#-------------------- generic flags --------------------
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CFLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CXXFLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${LDFLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS})

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)




#-------------------- glob all files --------------------
file(GLOB_RECURSE ALL_SRCS ${SRC_EXTS})
file(GLOB_RECURSE ALL_HEADERS ${HEADER_EXTS})
file(GLOB_RECURSE SUB_CMAKES */CMakeLists.txt)

#-------------------- sub cmake directorys --------------------
foreach(CMAKEFILE IN LISTS SUB_CMAKES)
	get_filename_component(DIR ${CMAKEFILE} DIRECTORY)
	#message("sub dir:${DIR}")
	add_subdirectory(${DIR})
	list(FILTER ALL_SRCS EXCLUDE REGEX "${DIR}/.*")
	list(FILTER ALL_HEADERS EXCLUDE REGEX "${DIR}/.*")
endforeach()


#-------------------- deps libs --------------------
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
foreach(DEP IN LISTS LIBS)
if(EXISTS ${CMAKE_MODULE_PATH}/Find${DEP}.cmake)
	find_package(${DEP})
endif()
endforeach()


#-------------------- exclude files --------------------
list(FILTER ALL_SRCS EXCLUDE REGEX "CMakeFiles")
list(FILTER ALL_HEADERS EXCLUDE REGEX "CMakeFiles")
foreach(EXCL IN LISTS EXCLUDES) 
	list(FILTER ALL_SRCS EXCLUDE REGEX ${EXCL})
	list(FILTER ALL_HEADERS EXCLUDE REGEX ${EXCL})
endforeach()

#-------------------- src types --------------------
set(HAS_TEST 0)
set(HAS_LIB 0)
set(HAS_MAIN 0)
foreach(SRC IN LISTS ALL_SRCS)
	get_filename_component(FILENAME ${SRC} NAME)	
	if(${FILENAME} MATCHES ${TEST_FILTER})
		list(APPEND TEST_SRCS ${SRC})
		set(HAS_TEST 1)
		#message("test:"  ${SRC})	
	elseif(${FILENAME} MATCHES ${MAIN_FILTER})
		list(APPEND MAIN_SRCS ${SRC})
		set(HAS_MAIN 1)
		#message("main:"  ${SRC})
	else()
		list(APPEND LIB_SRCS ${SRC})
		set(HAS_LIB 1)
		#message("lib :"  ${SRC})	
	endif()
endforeach()

#-------------------- includes --------------------
foreach(FILENAME IN LISTS ALL_HEADERS)
	get_filename_component(DIRNAME ${FILENAME} DIRECTORY)	
	if(NOT "${DIRNAME}" IN_LIST INCS)
		list(APPEND INCS ${DIRNAME})		
	endif()
endforeach()
foreach(INC IN LISTS INCS)
	include_directories(${INC})
endforeach()

#---------------- defines --------------
foreach(DEF IN LISTS DEFINES)
	add_definitions(-D${DEF})
endforeach()

#----------------------- external build ----------------------
include(${CONFIG_DIR}/build.cmake OPTIONAL)
if(EXISTS ${CONFIG_DIR}/build.cmake)	
	return()
endif()

#-- default build

 #-------------------- build lib --------------------
if(${HAS_MAIN})
	set(LIB_TARGET_NAME ${PROJECT_NAME}-lib)
else()
	set(LIB_TARGET_NAME ${PROJECT_NAME})
endif()
if(${HAS_LIB})
	add_library(${LIB_TARGET_NAME} ${LIB_SRCS})	
endif()

#-------------------- build main --------------------
set(MAIN_TARGET_NAME ${PROJECT_NAME})
if(${HAS_MAIN})
	add_executable(${MAIN_TARGET_NAME} ${MAIN_SRCS} ${RC_SRCS})
	if(${HAS_LIB})
		target_link_libraries(${MAIN_TARGET_NAME} ${LIB_TARGET_NAME} ${CONAN_LIBS})
		add_dependencies(${MAIN_TARGET_NAME} ${LIB_TARGET_NAME})
	endif()	
	foreach(DEP IN LISTS LIBS)		
		target_link_libraries(${MAIN_TARGET_NAME} ${DEP} )
		add_dependencies(${MAIN_TARGET_NAME} ${DEP})		
	endforeach()	
endif()



#-------------------- build unit test --------------------
set(TEST_TARGET_NAME ${PROJECT_NAME}-test)
if(${HAS_TEST} AND ${BUILD_TEST})
	add_executable(${TEST_TARGET_NAME} ${TEST_SRCS})
	if(${HAS_LIB})
		target_link_libraries(${TEST_TARGET_NAME} ${LIB_TARGET_NAME} ${CONAN_LIBS})
		add_dependencies(${TEST_TARGET_NAME} ${LIB_TARGET_NAME})
	endif()
	foreach(DEP IN LISTS LIBS)
		target_link_libraries(${TEST_TARGET_NAME} ${DEP} )
		add_dependencies(${TEST_TARGET_NAME} ${DEP})		
	endforeach()	
	target_compile_definitions(${TEST_TARGET_NAME} PUBLIC UNITTEST)
	target_compile_definitions(${TEST_TARGET_NAME} PUBLIC USE_GTEST)		
endif()






