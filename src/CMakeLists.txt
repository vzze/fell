set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    aux_source_directory(debug deb)
endif()

aux_source_directory(scanner scanner)
aux_source_directory(error error)

aux_source_directory(variable variable)
aux_source_directory(compiler compiler)

aux_source_directory(vm vm)
aux_source_directory(std std)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(libfell interpreter.cc ${compiler} ${error} ${scanner} ${vm} ${variable} ${std} ${deb})
else()
    add_library(libfell interpreter.cc ${compiler} ${error} ${scanner} ${vm} ${variable} ${std})
endif()

add_executable(fell fell.cc)
target_link_libraries(fell libfell)
