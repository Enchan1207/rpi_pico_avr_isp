cmake_minimum_required(VERSION 3.12)

add_subdirectory(core)

if(${BUILD_FOR_TARGET})
    add_subdirectory(app)
endif()
