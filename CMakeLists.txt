cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Testing setup
# based on example in https://github.com/udacity/CppND-Route-Planning-Project

# Add the GoogleTest library subdirectory
add_subdirectory(thirdparty/googletest)

# Add the testing executable
add_executable(test
    test/test_tests.cpp
    test/test_linux_parser_pure.cpp
    test/test_linux_parser_integration.cpp
    test/test_format.cpp
    src/linux_parser_pure.cpp
    src/linux_parser.cpp
    src/format.cpp)

target_link_libraries(test gmock_main)

set_property(TARGET test PROPERTY CXX_STANDARD 17)
