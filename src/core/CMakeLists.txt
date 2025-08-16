cmake_minimum_required(VERSION 3.12)

add_library(core)

target_sources(core PRIVATE
    parser.c
    handler/handler.c
    handler/sync.c
    handler/device.c
    handler/program.c
    handler/universal.c
    handler/flash.c
    handler/eeprom.c
    handler/fuse.c
)
