cmake_minimum_required(VERSION 3.9.4)

project(fell)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

if(MSVC)
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(
            CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
            "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
        )
    endif()

    add_compile_options(
        /W3 /WX
    )
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    add_compile_options(
        -Wall -Wextra -Wpedantic -Wconversion -Werror
    )
endif()

add_compile_definitions(
    $<$<CONFIG:RelWithDebInfo>:FELL_DEBUG>
    $<$<CONFIG:Debug>:FELL_DEBUG>
)

add_subdirectory(src)
