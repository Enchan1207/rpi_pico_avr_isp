cmake_minimum_required(VERSION 3.12)

add_executable(isp)

target_sources(isp PRIVATE
    main.c
)

target_link_libraries(isp
    pico_stdlib
    hardware_spi
    core
)

# stdioをUSBに接続する
pico_enable_stdio_usb(isp 1)
pico_enable_stdio_uart(isp 0)

pico_add_extra_outputs(isp)
