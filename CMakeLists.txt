cmake_minimum_required(VERSION 3.12)

include(cmake/ProjectOptions.cmake)

project (
    fell
    VERSION 0.1.6
    DESCRIPTION "Embeddable Interpreted Language"
)

set_project_options(20 17 "src")

add_subdirectory("src")
