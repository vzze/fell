set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

aux_source_directory(scanner scanner)
aux_source_directory(error error)

aux_source_directory(variable variable)
aux_source_directory(compiler compiler)

aux_source_directory(vm vm)
aux_source_directory(std std)

add_library(libfell interpreter.cc ${compiler} ${error} ${scanner} ${vm} ${variable} ${std})

add_executable(fell fell.cc)
target_link_libraries(fell libfell)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    aux_source_directory(debug deb)
    add_library(debugfell ${deb})

    target_link_libraries(fell debugfell)
else()
    file(GLOB_RECURSE files "${CMAKE_CURRENT_SOURCE_DIR}/../tests/runtime/*.fell")
    set(i 0)

    foreach(filename ${files})
        get_filename_component(no_extension ${filename} NAME_WE)
        add_test(NAME "${no_extension}_${i}" COMMAND $<TARGET_FILE:fell> ${filename})
        math(EXPR i "${i} + 1")
    endforeach()
endif()
