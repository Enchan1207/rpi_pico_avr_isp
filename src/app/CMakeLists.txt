cmake_minimum_required(VERSION 3.12)

add_executable(rpi_pico_avr_isp)

target_sources(rpi_pico_avr_isp PRIVATE
    main.c
)

target_link_libraries(rpi_pico_avr_isp
    pico_stdlib
    hardware_spi
    core
)

# stdioをUSBに接続する
pico_enable_stdio_usb(rpi_pico_avr_isp 1)
pico_enable_stdio_uart(rpi_pico_avr_isp 0)

pico_add_extra_outputs(rpi_pico_avr_isp)
