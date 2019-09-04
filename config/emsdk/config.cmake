set(CMAKE_SYSTEM_NAME Generic)

find_program(EMSDK_PROG "emsdk")
if(NOT EXISTS ${EMSDK_PROG})
    message(FATAL_ERROR "Can not find emsdk")
endif()

get_filename_component(EMSDK ${EMSDK_PROG} DIRECTORY)

if(${CMAKE_HOST_WIN32})
set(CMAKE_C_COMPILER "${EMSDK}/fastcomp/emscripten/emcc.bat" )
set(CMAKE_CXX_COMPILER "${EMSDK}/fastcomp/emscripten/em++.bat" )
else()
set(CMAKE_C_COMPILER "${EMSDK}/fastcomp/emscripten/emcc" )
set(CMAKE_CXX_COMPILER "${EMSDK}/fastcomp/emscripten/em++" )
endif()


set(FLAGS "-s WASM=1")
