#
# Blink
#

cmake_minimum_required(VERSION 3.12)

## configure executable
add_executable(rpi_pico_avr_isp)
target_sources(rpi_pico_avr_isp PRIVATE
    main.c
)
target_include_directories(rpi_pico_avr_isp PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(rpi_pico_avr_isp PRIVATE
    -Wall
)
target_link_libraries(rpi_pico_avr_isp
    pico_stdlib
    hardware_spi
)

pico_add_extra_outputs(rpi_pico_avr_isp)
