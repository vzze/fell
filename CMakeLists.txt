set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 20)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")

cmake_minimum_required(VERSION 3.5)

project(fell)

include_directories(
    include
)

add_subdirectory(src/variable)
add_subdirectory(src/util)

add_subdirectory(src/std)
add_subdirectory(src/lexer)

add_subdirectory(src/api)

add_library(libfell src/lib.cc)
target_link_libraries(libfell lexer std util variable api)

add_executable(fell src/interpreter.cc)
target_link_libraries(fell libfell)
