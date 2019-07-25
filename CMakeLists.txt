cmake_minimum_required(VERSION 3.7)

# CC CXX CFLAGS CXXFLAGS LDFLAGS

#--------------------- generic settings ----------------------
 
file(RELATIVE_PATH PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR})
#set(PROJECT_NAME "proj")

set(SRC_EXTS *.cpp *.c *.cc)

set(HEADER_EXTS *.h *.hpp)

#list(APPEND EXLUDES ".*CMakeFiles.*")

set(FLAGS )
set(CFLAGS ${FLAGS})
set(CXXFLAGS ${FLAGS})

set(LDFLAGS "")

set(LIBS "-lz")

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


#-------------------- conan packages --------------------
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/conanfile.txt)
	if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		conan_global_flags()
		conan_define_targets()		
		conan_set_find_paths()
		conan_set_find_library_paths()
	else()		
		message("Please run install conan package first")
		message("Example: conan install .. -s build_type=Release --build=missing -g cmake")
		message("See https://docs.conan.io/en/latest/index.html")
		message(FATAL_ERROR "package not installed")		
	endif()
endif()

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
endif()

#-------------------- build unit test --------------------
set(TEST_TARGET_NAME ${PROJECT_NAME}-test)
if(${HAS_TEST})
	add_executable(${TEST_TARGET_NAME} ${TEST_SRCS})
	if(${HAS_LIB})
		target_link_libraries(${TEST_TARGET_NAME} ${LIB_TARGET_NAME} ${CONAN_LIBS})
		add_dependencies(${TEST_TARGET_NAME} ${LIB_TARGET_NAME})
	endif()
	target_compile_definitions(${TEST_TARGET_NAME} PUBLIC UNITTEST)
	target_compile_definitions(${TEST_TARGET_NAME} PUBLIC USE_GTEST)		
endif()

