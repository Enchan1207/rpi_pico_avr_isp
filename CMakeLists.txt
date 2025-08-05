#
# CMake project for Raspberry Pi Pico
#
# reference: https://github.com/raspberrypi/pico-examples/blob/master/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.12)

## picoのSDKを読み込んでおく (おそらくツールチェインファイルなので、 -DCMAKE_TOOLCHAIN_FILE でも良さそう)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

## プロジェクト構成
project(rpi_pico_avr_isp
    VERSION 0.1.0
    DESCRIPTION "Make Raspberry Pi Pico to AVR ISP"
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## SDK初期化
pico_sdk_init()

## 全ターゲット共通のコンパイルオプション
add_compile_options(
    -Wno-format
    -Wno-unused-function
)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

## ソースディレクトリ
add_subdirectory(src)
