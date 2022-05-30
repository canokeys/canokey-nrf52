cmake_minimum_required(VERSION 3.18)

set(CROSS_COMPILE /usr/bin/arm-none-eabi-)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake)

project(canokey-nrf52 C ASM)
set(TARGET canokey)

set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/)
set(THIRD_PARTY_DIR ${BASE_DIR}/third-party)

set(LINKER_SCRIPT "${BASE_DIR}/nrf52840.ld")

add_definitions(-DNRF52840_XXAA)
add_definitions(-DCFG_BOARD=OPT_BOARD_MAKERDIARY_M2_DOCK)
add_definitions(-DCFG_TUSB_MCU=OPT_MCU_NRF5X)
add_definitions(-DMBEDTLS_CONFIG_FILE="${BASE_DIR}/config/mbedtls_config.h")

set(CMAKE_C_STANDARD 11)
set(COMMON_COMPILE_FLAGS "-mcpu=cortex-m4 -mthumb")

set(WARNING_FLAGS "-Wall")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_COMPILE_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB_RECURSE NRFX_SRCS
    "${THIRD_PARTY_DIR}/nrfx/drivers/src/*.c"
)

file(GLOB SRCS
"${BASE_DIR}/src/*.c"
)

set(SOURCES
    "${THIRD_PARTY_DIR}/nrfx/mdk/gcc_startup_nrf52840.S"
    "${THIRD_PARTY_DIR}/nrfx/mdk/system_nrf52.c"
    "${SRCS}"
)

add_executable(${TARGET} ${SOURCES} ${NRFX_SRCS})

# Include dirs
set(CMSIS_INCLUDE_DIRS
    "${THIRD_PARTY_DIR}/CMSIS/CMSIS/Core/Include"
)

set(NRFX_INCLUDE_DIRS
    "${THIRD_PARTY_DIR}/nrfx"
    "${THIRD_PARTY_DIR}/nrfx/hal"
    "${THIRD_PARTY_DIR}/nrfx/mdk"
    "${THIRD_PARTY_DIR}/nrfx/drivers/include/"
    "${THIRD_PARTY_DIR}/nrfx/drivers/src/"
)

target_include_directories(${TARGET} PRIVATE ${CMSIS_INCLUDE_DIRS})
target_include_directories(${TARGET} PRIVATE ${NRFX_INCLUDE_DIRS})
target_include_directories(${TARGET} PRIVATE "${BASE_DIR}/include")
target_include_directories(${TARGET} PRIVATE "${BASE_DIR}/config")

# TinyUSB DCD configure
set(DCD_INCLUDE_DIRS
    ${CMSIS_INCLUDE_DIRS}
    ${NRFX_INCLUDE_DIRS}
    "${BASE_DIR}/config"
)

set(TINYUSB_MCU "OPT_MCU_NRF5X" CACHE STRING "TinyUSB MCU name")
set(TINYUSB_DCD_INCLUDES ${DCD_INCLUDE_DIRS} CACHE STRING "TinyUSB DCD include paths")

# CanoKey Core
add_subdirectory(canokey-core)
target_link_libraries(${TARGET} PRIVATE
    -Wl,--whole-archive
    canokey-core
    -Wl,--no-whole-archive
)
target_link_libraries(${TARGET} PRIVATE c m nosys)

# Generate Git version header
add_dependencies(${TARGET} gitrev)
set(gitrev_in include/git-rev.h.in)
set(gitrev include/git-rev.h)
add_custom_target(gitrev
    ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_SOURCE_DIR}/${gitrev}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${gitrev_in} ${CMAKE_CURRENT_SOURCE_DIR}/${gitrev}
    COMMAND git describe --always --tags --long --abbrev=8 --dirty >> ${CMAKE_CURRENT_SOURCE_DIR}/${gitrev}
)

# Create a binary from the elf
add_custom_target(${TARGET}.bin
    DEPENDS ${TARGET}
    COMMAND "${CROSS_COMPILE}objcopy" -O binary -S ${TARGET} ${TARGET}.bin
)

add_custom_target(${TARGET}.hex
    DEPENDS ${TARGET}
    COMMAND "${CROSS_COMPILE}objcopy" -O ihex -S ${TARGET} ${TARGET}.hex
)

# Print size of the elf
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CROSS_COMPILE}size ${TARGET}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()


# vim: set ts=4 sw=4 sts=4 et smarttab :
