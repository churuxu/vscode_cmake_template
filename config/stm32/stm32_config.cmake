
set(CMAKE_SYSTEM_NAME Generic )
set(CMAKE_C_COMPILER "arm-none-eabi-gcc" )
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++" )
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc" )
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs"  )
set(CMAKE_ASM_FLAGS " -x assembler-with-cpp")
set(CMAKE_AR "arm-none-eabi-ar" )
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")
set(CMAKE_SIZE "arm-none-eabi-size")

enable_language(ASM)

add_compile_options(-mcpu=${CPU_TYPE} -mthumb -ffunction-sections -fdata-sections -nostdlib )

