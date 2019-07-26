cmake_minimum_required(VERSION 3.7)

### cmake template for build package with local source ### 



#-------------------- generic setting --------------------
 
file(RELATIVE_PATH PACKAGE_NAME ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})


set(SRC_EXTS *.cpp *.c *.cc)

set(HEADER_EXTS *.h *.hpp)

#list(APPEND EXLUDES ".*CMakeFiles.*")

set(FLAGS )
set(CFLAGS ${FLAGS})
set(CXXFLAGS ${FLAGS})

set(LDFLAGS "")

set(LIBS "")

#for windows gui application  (1=gui 0=console)
#set(NO_CONSOLE 1)

#main source file name (regex)
#set(MAIN_FILTER "^main\..*")

#test source file name (regex)
#set(TEST_FILTER ".*test.*\..*")


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


#-------------------- platform speical --------------------
if(${WIN32})
	file(GLOB_RECURSE RC_SRCS "*.rc")
	add_compile_options(-DUNICODE -D_UNICODE)
	link_libraries(-lcomctl32 -lpsapi -lws2_32)
	if(${MINGW} AND ${NO_CONSOLE})		
		set(LDFLAGS "${LDFLAGS} -Wl,--subsystem,windows")		
	endif()
endif()


#-------------------- compiler speical --------------------

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC) #vc
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
else() #gcc
	add_compile_options(-Wall)
endif()


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
file(GLOB_RECURSE ALL_SRCS ${SRC_EXTS})
file(GLOB_RECURSE ALL_HEADERS ${SRC_EXTS})
file(GLOB_RECURSE SUB_CMAKES */CMakeLists.txt)


#-------------------- exlude files --------------------
list(FILTER ALL_SRCS EXCLUDE REGEX "CMakeFiles")
list(FILTER ALL_HEADERS EXCLUDE REGEX "CMakeFiles")
foreach(EXLUDE IN LISTS EXLUDES) 
	list(FILTER ALL_SRCS EXCLUDE REGEX ${EXLUDE})
	list(FILTER ALL_HEADERS EXCLUDE REGEX ${EXLUDE})
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


#-------------------- build lib --------------------
set(LIB_TARGET_NAME ${PACKAGE_NAME})
if(${HAS_LIB})
	add_library(${LIB_TARGET_NAME} ${LIB_SRCS})
	#export(TARGETS ${LIB_TARGET_NAME} FILE ${CMAKE_BINARY_DIR}/${LIB_TARGET_NAME}_targets.cmake)
endif()

#-------------------- export info --------------------
get_target_property(BUILD_DIR ${LIB_TARGET_NAME} BINARY_DIR)

set(EXPORT_INFO "")
set(EXPORT_INFO "${EXPORT_INFO}#package ${PACKAGE_NAME}\n")
set(EXPORT_INFO "${EXPORT_INFO}link_directories(${BUILD_DIR})\n")
#set(EXPORT_INFO "${EXPORT_INFO}link_libraries(${PACKAGE_TARGET})\n")
foreach(DIR IN LISTS INCS)
	set(EXPORT_INFO "${EXPORT_INFO}include_directories(${DIR})\n")
endforeach()

set(EXPORT_FILE ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}.exports.cmake)

if(EXISTS ${EXPORT_FILE})
	file(READ ${EXPORT_FILE} OLD_INFO)
endif()
if(NOT "${OLD_INFO}" STREQUAL "${EXPORT_INFO}")
	file(WRITE ${EXPORT_FILE} "${EXPORT_INFO}")
endif()





