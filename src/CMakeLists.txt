if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    add_subdirectory(debug)
endif()

add_subdirectory(scanner)
add_subdirectory(error)

add_subdirectory(variable)
add_subdirectory(compiler)

add_subdirectory(vm)
add_subdirectory(std)

add_library(libfell interpreter.cc)
target_link_libraries(libfell compiler error scanner vm variable std)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(libfell deb)
endif()

add_executable(fell fell.cc)
target_link_libraries(fell libfell)
