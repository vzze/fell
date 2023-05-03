set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEBUG)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

cmake_minimum_required(VERSION 3.2)

project(fell)

if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Werror")

        if(DEBUG)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
        endif()

    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Werror")

    if(DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
    endif()
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")

    if(DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"DEBUG\"")
    endif()
endif()

include_directories(
    include
)

add_subdirectory(src)

add_library(libfell src/interpreter.cc)
target_link_libraries(libfell compiler error scanner vm variable std)

if(DEBUG)
    target_link_libraries(libfell deb)
endif()

add_executable(fell src/fell.cc)
target_link_libraries(fell libfell)
