cmake_minimum_required(VERSION 3.18)

set(CROSS_COMPILE /usr/bin/arm-none-eabi-)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake)

project(canokey-nrf52 C ASM)
set(TARGET canokey)

# board selection, see config/board_selection.h for avaliable boards
set(CANOKEY_BOARD OPT_BOARD_NRF52840_DONGLE)
# set(CANOKEY_BOARD OPT_BOARD_MAKERDIARY_M2_DOCK)

set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/)
set(NRF5_SDK_DIR ${BASE_DIR}/nrf_sdk/nRF5_SDK_17.1.0_ddde560)

set(LINKER_SCRIPT "${BASE_DIR}/nrf52840.ld")

add_definitions(-DNRF52840_XXAA)
add_definitions(-DMBR_PRESENT)
add_definitions(-DCFG_BOARD=${CANOKEY_BOARD})
add_definitions(-DCFG_TUSB_MCU=OPT_MCU_NRF5X)
add_definitions(-DMBEDTLS_CONFIG_FILE="${BASE_DIR}/config/mbedtls_config.h")

# nrf5 sdk configs
add_definitions(-DCUSTOM_BOARD_INC=board_selection)
add_definitions(-DBSP_DEFINES_ONLY)
add_definitions(-DCONFIG_GPIO_AS_PINRESET)
add_definitions(-DFLOAT_ABI_HARD)

set(CMAKE_C_STANDARD 11)
set(COMMON_COMPILE_FLAGS "-mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(EXTRA_COMPILE_FLAGS "-ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin -fshort-enums -g3 -O0")

set(WARNING_FLAGS "-Wall")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${EXTRA_COMPILE_FLAGS} ${DEFINITIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${EXTRA_COMPILE_FLAGS} ${DEFINITIONS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_COMPILE_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${NRF5_SDK_DIR}/modules/nrfx/mdk -T ${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${TARGET}.map --specs=nano.specs")

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${BASE_DIR}/nrf_sdk/nRF5_SDK.cmake)

file(GLOB SRCS
    "${BASE_DIR}/src/*.c"
)

set(SOURCES
    ${NRF5_SDK_SOURCES}
    ${SRCS}
)

add_executable(${TARGET} ${SOURCES})

# Include dirs
target_include_directories(${TARGET} PRIVATE ${NRF5_SDK_INCLUDES})
target_include_directories(${TARGET} PRIVATE ${BASE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${BASE_DIR}/config)

# nrf sdk binary libraries
target_link_libraries(${TARGET} PRIVATE ${NRF5_SDK_LIBRARIES})

# TinyUSB DCD configure
set(DCD_INCLUDE_DIRS
    ${NRF5_SDK_INCLUDES}
    ${BASE_DIR}/config
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

# Generate UF2 package for Adafruit bootloader
add_custom_target(${TARGET}_flash.uf2
    DEPENDS ${TARGET}.hex
    COMMAND python ${BASE_DIR}/utils/uf2/utils/uf2conv.py --convert --family 0xADA52840 -o ${TARGET}_flash.uf2 ${TARGET}.hex
)

# NRF sdk config tool
set(NRF5_SDK_CONFIG_FILE ${BASE_DIR}/config/sdk_config.h)
set(CMSIS_CONFIG_TOOL ${NRF5_SDK_DIR}/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar)

add_custom_target(sdk_config
    COMMAND java -jar ${CMSIS_CONFIG_TOOL} ${NRF5_SDK_CONFIG_FILE}
)

# auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()


# vim: set ts=4 sw=4 sts=4 et smarttab :
