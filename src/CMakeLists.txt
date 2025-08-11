cmake_minimum_required(VERSION 3.12)

add_executable(rpi_pico_avr_isp)

target_sources(rpi_pico_avr_isp PRIVATE
    main.c
    stk500.c
    avr_isp.c
)

target_link_libraries(rpi_pico_avr_isp
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(rpi_pico_avr_isp)
