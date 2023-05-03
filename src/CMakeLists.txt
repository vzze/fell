add_subdirectory(scanner)
add_subdirectory(error)

add_subdirectory(variable)
add_subdirectory(compiler)

add_subdirectory(vm)
add_subdirectory(std)

if(DEBUG)
    add_subdirectory(debug)
endif()
