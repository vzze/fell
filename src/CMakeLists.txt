if(PROJECT_IS_TOP_LEVEL)
    add_executable(fell "fell/fell.cc")
    target_link_libraries(
        fell
        PRIVATE fell_options
        PRIVATE fell_warnings
    )
endif()
