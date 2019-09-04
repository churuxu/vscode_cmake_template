

#-------------------- build wasm --------------------

add_executable(${PROJECT_NAME}.wasm ${LIB_SRCS} ${MAIN_SRCS})

foreach(DEP IN LISTS LIBS)		
	target_link_libraries(${PROJECT_NAME}.wasm  ${DEP} )
	add_dependencies(${PROJECT_NAME}.wasm  ${DEP})		
endforeach()	

