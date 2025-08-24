cmake_minimum_required(VERSION 3.12)

if($ENV{ENV_NAME} STREQUAL "rp2040")
    add_subdirectory(rp2040)
endif()

if($ENV{ENV_NAME} STREQUAL "avr")
    add_subdirectory(avr)
endif()
