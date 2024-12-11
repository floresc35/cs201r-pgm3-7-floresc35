cmake_minimum_required(VERSION 3.10)
project(project-name)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(tests)

include(GoogleTest)

add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION /path/to/libgtest.a)

add_executable(runTests tests/test_main.cpp)
target_link_libraries(runTests gtest gtest_main)
gtest_discover_tests(runTests)