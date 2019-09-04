set(CMAKE_SYSTEM_NAME Generic)

find_program(EMSDK_PROG "emsdk")
if(NOT EXISTS ${EMSDK_PROG})
    message(FATAL_ERROR "Can not find emsdk")
endif()

get_filename_component(EMSDK ${EMSDK_PROG} DIRECTORY)

if(${CMAKE_HOST_WIN32})
set(PROG_EXT .bat)
else()
set(PROG_EXT )
endif()

set(CMAKE_C_COMPILER "${EMSDK}/fastcomp/emscripten/emcc${PROG_EXT}" )
set(CMAKE_CXX_COMPILER "${EMSDK}/fastcomp/emscripten/em++${PROG_EXT}" )
set(CMAKE_AR "${EMSDK}/fastcomp/emscripten/emar${PROG_EXT}" )


add_compile_options(-s WASM=1)

