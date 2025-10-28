cmake_minimum_required(VERSION 3.16)
project(UnitTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

enable_testing()

add_library(library STATIC
        library.cpp
        library.h)

add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE library gtest gtest_main)


include(GoogleTest)
add_test(NAME AllTests COMMAND tests)
