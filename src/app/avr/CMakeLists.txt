cmake_minimum_required(VERSION 3.12)

add_executable(isp)

target_sources(isp PRIVATE
    main.c
)

target_link_libraries(isp
    core
)

target_configure_for_avr(isp)
