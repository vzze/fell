include(CheckIPOSupported)

include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)

macro(set_parallel_level)
    cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_PHYSICAL_CORES)

    if(${CORES} EQUAL 0)
        message(STATUS "${PROJECT_NAME}: Build in parallel disabled")
    else()
        set(CMAKE_BUILD_PARALLEL_LEVEL ${CORES})
        message(STATUS "${PROJECT_NAME}: Build in parallel enabled with ${CORES} cores")
    endif()
endmacro()

macro(enable_ipo)
    check_ipo_supported(RESULT SUPPORTED OUTPUT ERROR)

    if(SUPPORTED)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            find_program(GCC_AR gcc-ar)

            if(GCC_AR)
                message(STATUS "${PROJECT_NAME}: GCC detected: setting up CMake AR")

                set(CMAKE_AR ${GCC_AR})
                set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
                set(CMAKE_C_ARCHIVE_FINISH true)

                set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
                message(STATUS "${PROJECT_NAME}: IPO / LTO Enabled")
            else()
                message(SEND_ERROR "${PROJECT_NAME}: gcc-ar is needed for LTO optimization")
            endif()
        else()
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

            if(MSVC)
                add_compile_options(/Gy)
            endif()

            message(STATUS "${PROJECT_NAME}: IPO / LTO Enabled")
        endif()
    else()
        message(STATUS "${PROJECT_NAME}: IPO / LTO not supported: ${ERROR}")
    endif()
endmacro()

macro(enable_msvc_hot_reload)
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endmacro()

macro(set_project_options cxx_standard c_standard include_dir)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_parallel_level()
    enable_ipo()
    enable_msvc_hot_reload()
    enable_clang_tidy()

    add_library(${PROJECT_NAME}_warnings INTERFACE)
    set_compiler_warnings(${PROJECT_NAME}_warnings)

    add_library(${PROJECT_NAME}_options INTERFACE)

    message(STATUS "${PROJECT_NAME}: Include directories set to: ${include_dir}")

    target_include_directories(${PROJECT_NAME}_options INTERFACE ${include_dir})

    target_compile_features(${PROJECT_NAME}_options INTERFACE cxx_std_${cxx_standard})
    target_compile_features(${PROJECT_NAME}_options INTERFACE c_std_${c_standard})

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endmacro()
