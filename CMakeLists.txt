cmake_minimum_required(VERSION 3.16)
project(tracker_case CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tracker src/association.cpp)
target_include_directories(tracker PUBLIC include)
target_compile_options(tracker PRIVATE -Wall -Wextra -Wpedantic)

add_library(doctest INTERFACE)
target_include_directories(doctest SYSTEM INTERFACE third_party)

enable_testing()
add_executable(association_test tests/association_test.cpp)
target_link_libraries(association_test PRIVATE tracker doctest)
add_test(NAME association_test COMMAND association_test)
