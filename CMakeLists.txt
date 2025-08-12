cmake_minimum_required(VERSION 3.12)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    set(BUILD_FOR_TARGET true)
else()
    set(BUILD_FOR_TARGET false)
endif()

if(${BUILD_FOR_TARGET})
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

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

if(${BUILD_FOR_TARGET})
    pico_sdk_init()
endif()

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

if(NOT ${BUILD_FOR_TARGET})
    enable_testing()
    add_subdirectory(tests)
endif()
