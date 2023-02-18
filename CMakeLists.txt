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

if(BUILD_FELL)
    add_subdirectory(src/variable)
    add_subdirectory(src/util)

    add_subdirectory(src/lang)
    add_subdirectory(src/lexer)

    add_library(libfell src/lib.cc)
    target_link_libraries(libfell lexer lang util variable)

    add_executable(fell src/interpreter.cc)
    target_link_libraries(fell libfell)
endif()


if(BUILD_TESTS)
    include_directories(
        build
    )

    aux_source_directory(tests TESTS)

    foreach(test ${TESTS})
        string(REPLACE ".cc" "" new_name ${test})
        get_filename_component(file_name ${new_name} NAME)

        add_executable(${file_name} ${test})
        target_link_libraries(${file_name} libfell)
    endforeach()
endif(BUILD_TESTS)
