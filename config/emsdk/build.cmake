

#-------------------- build wasm --------------------

set(MAIN_TARGET ${PROJECT_NAME}.wasm) 

add_executable(${MAIN_TARGET} ${LIB_SRCS} ${MAIN_SRCS})

foreach(DEP IN LISTS LIBS)		
	target_link_libraries(${MAIN_TARGET}  ${DEP} )
	add_dependencies(${MAIN_TARGET}  ${DEP})		
endforeach()	

