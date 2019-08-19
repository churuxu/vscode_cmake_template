

#-------------------- build so --------------------

set(MAIN_TARGET_NAME ${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED ${LIB_SRCS})

foreach(DEP IN LISTS LIBS)		
	target_link_libraries(${PROJECT_NAME} ${DEP} )
	add_dependencies(${PROJECT_NAME} ${DEP})		
endforeach()	

