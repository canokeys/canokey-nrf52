# A CMake-template for cross-compiling for STM32 (or other) ARM Cortex-M MCUs.
# Copyright (C) 2016  Andreas Hofmann
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 3.18)

project(tusb-demo C CXX ASM)
set(TARGET tusb-demo)

# General settings {{{

set(CROSS_TARGET_TRIPLET "arm-none-eabi-")
set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/)
set(THIRD_PARTY_DIR ${BASE_DIR}/third-party)

# Fill in the target CPU here
set(CPU "cortex-m4")

# Fill in the linker script to use here
set(LINKER_SCRIPT "${BASE_DIR}/nrf52840.ld")

# Definitions {{{

# We're not using add_definitions() here, since this would break the calls to the assembler.
# Some examples:
set(DEFINITIONS "${DEFINITIONS} -DNRF52840_XXAA")

#}}}

# Compiler settings {{{

# Prepend the cross-compiler prefix to the used tools.
set(CMAKE_C_COMPILER "${CROSS_TARGET_TRIPLET}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_TARGET_TRIPLET}g++")
set(CMAKE_ASM_COMPILER "${CROSS_TARGET_TRIPLET}gcc")

# Be verbose when compiling.
set(WARNING_FLAGS "-Wall")

# Flags which are required for gcc, g++ and as.
set(COMMON_COMPILE_FLAGS "-mcpu=${CPU} -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} ${WARNING_FLAGS} ${DEFINITIONS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_COMPILE_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

# The following is required to get rid of the (not supported) -rdynamic flag
# when linking the final binary.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" )
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#}}}

# Source files  {{{

file(GLOB_RECURSE NRFX_SRCS
    "${THIRD_PARTY_DIR}/nrfx/drivers/src/*.c"
)

file(GLOB_RECURSE TINYUSB_SRCS
    "${THIRD_PARTY_DIR}/tinyusb/src/*.c"
)

# Fill in your source files here (duh).
set(SOURCES
    "${THIRD_PARTY_DIR}/nrfx/mdk/gcc_startup_nrf52840.S"
    "${THIRD_PARTY_DIR}/nrfx/mdk/system_nrf52.c"

    "${BASE_DIR}/src/main.c"
    "${BASE_DIR}/src/board.c"
    "${BASE_DIR}/src/usb_descriptors.c"
)

#}}}

# Include dirs  {{{

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

set(TINYUSB_INCLUDE_DIRS
    "${THIRD_PARTY_DIR}/tinyusb/src"
)

# Fill in the directories of your header files here.
include_directories(
    ${CMSIS_INCLUDE_DIRS}
    ${NRFX_INCLUDE_DIRS}
    ${TINYUSB_INCLUDE_DIRS}
)

include_directories(
    "${BASE_DIR}/include"
    "${BASE_DIR}/config"
)

#}}}

# Build the image {{{

# Compile the sources to an .elf
add_executable(${TARGET}.elf ${SOURCES} ${NRFX_SRCS} ${TINYUSB_SRCS})

# Create a binary from the elf
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CROSS_TARGET_TRIPLET}objcopy -Oihex ${TARGET}.elf ${TARGET}.hex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# print size of the elf
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CROSS_TARGET_TRIPLET}size ${TARGET}.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#}}}

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()


# vim: set ts=4 sw=4 sts=4 et smarttab :
