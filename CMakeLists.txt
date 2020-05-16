cmake_minimum_required(VERSION 3.10)
project(frequenzzaehler)

set(AVRSTRIP avr-strip)
set(OBJCOPY avr-objcopy)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

set(CWARN "-Wall -Wextra -Wstrict-prototypes") #warnungen des compilers an
set(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums") #optimierungen
set(COPT "-Os") #optimiert für speicher (Os), optimiert für performance (O3)
set(CMCU "-mmcu=attiny85")
set(CMCUAVRNAME "t85")
set(CDEFS "-DF_CPU=128000ul") # clock at 128kHz
set(CFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT} ${CWARN}")
set(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT} ${CWARN}")
set(CMAKE_C_FLAGS  ${CFLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(DRIVER_DIR ../max7219/max7219_driver)
set(DRIVER_FILES ${DRIVER_DIR}/max7219_driver.h ${DRIVER_DIR}/max7219_driver.c)
set(SOURCES main.c ${DRIVER_FILES})

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
add_custom_target(strip ALL ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}.hex   ALL ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(compile_and_flash DEPENDS ${PROJECT_NAME}.hex)