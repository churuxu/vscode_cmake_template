set(PROJNAME ${PROJECT_NAME})


# ------------- build application -------------
add_executable(${PROJNAME}.elf ${MAIN_SRCS} ${LIB_SRCS} )
foreach(DEP IN LISTS LIBS)		
	target_link_libraries(${PROJNAME}.elf ${DEP} )
	add_dependencies(${PROJNAME}.elf ${DEP})		
endforeach()

target_link_libraries(${PROJNAME}.elf "-T${LDSCRIPT} -lc_nano -lm -lnosys -mcpu=cortex-m3 -mthumb -Wl,-Map=${PROJNAME}.map,--cref -Wl,--gc-sections")
set_target_properties(${PROJNAME}.elf PROPERTIES LINK_DEPENDS ${LDSCRIPT})
add_custom_command(TARGET ${PROJNAME}.elf  POST_BUILD COMMAND ${CMAKE_SIZE} ${PROJNAME}.elf )

add_custom_command(OUTPUT ${PROJNAME}.bin DEPENDS ${PROJNAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJNAME}.elf ${PROJNAME}.bin)
add_custom_target(exportbin ALL DEPENDS ${PROJNAME}.bin)

# ------------- flash command -------------

set(JLINKCMD "r\r\n")
set(JLINKCMD "${JLINKCMD}h\r\n")
set(JLINKCMD "${JLINKCMD}loadfile ${CMAKE_BINARY_DIR}/${PROJNAME}.bin 0x08000000\r\n")
set(JLINKCMD "${JLINKCMD}g\r\n")
set(JLINKCMD "${JLINKCMD}q\r\n")
file(WRITE ${CMAKE_BINARY_DIR}/jlink.txt ${JLINKCMD})

add_custom_target(flash DEPENDS ${PROJNAME}.elf COMMAND JLink -device ${CPU_NAME} -if JTAG -speed auto -CommanderScript ${CMAKE_BINARY_DIR}/jlink.txt)

# ------------- gdb server command -------------

set(GDB_SERVER_CMD "JLinkGDBServerCL -nogui -singlerun -select USB -device ${CPU_NAME} -endian little -if JTAG -speed auto -noir -LocalhostOnly")

add_custom_target(gdb_server DEPENDS ${PROJNAME}.elf COMMAND ${GDB_SERVER_CMD})

file(WRITE ${CMAKE_BINARY_DIR}/gdb_server.bat "start /MIN ${GDB_SERVER_CMD}")

