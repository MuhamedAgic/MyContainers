cmake_minimum_required(VERSION 3.14)
project(MySet)

set(CMAKE_CXX_STANDARD 20)

# Main executable
add_executable(MySet main.cpp UnorderedSet.h)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable
add_executable(MySetTest MySetTest.cpp UnorderedSet.h)
target_link_libraries(MySetTest gtest_main)

add_test(NAME MySetUnitTests COMMAND MySetTest)