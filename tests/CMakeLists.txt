cmake_minimum_required(VERSION 3.12)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(core_test)
target_sources(core_test PRIVATE
    test_parser.cpp
    handler/test_handler.cpp
    handler/common.cpp
    handler/test_sync.cpp
    handler/test_universal.cpp
    handler/test_program.cpp
    handler/test_fuse.cpp
    handler/test_device.cpp
    handler/test_eeprom.cpp
    handler/test_flash.cpp
)
target_link_libraries(core_test PRIVATE
    GTest::gtest_main
    core
)
target_include_directories(core_test PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

gtest_discover_tests(core_test)
