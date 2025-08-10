#
# CMake project for Raspberry Pi Pico
#
# reference: https://github.com/raspberrypi/pico-examples/blob/master/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(rpi_pico_avr_isp
    VERSION 0.1.0
    DESCRIPTION "Make Raspberry Pi Pico to AVR ISP"
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

pico_sdk_init()

add_compile_options(
    -Wno-format
    -Wno-unused-function
    -Wall
)

include_directories(BEFORE
    ${PROJECT_SOURCE_DIR}/include
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_subdirectory(src)
