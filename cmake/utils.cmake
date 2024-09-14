function(ADD_HW NUMBER)
    add_subdirectory(HW${NUMBER})
    target_link_libraries(${PROJECT_NAME} HW${NUMBER}_LIB)

    if (MSVC)
        target_compile_options(HW${NUMBER}_LIB PRIVATE
            /W4
        )
    else ()
        target_compile_options(HW${NUMBER}_LIB PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()

    set_target_properties(HW${NUMBER}_LIB PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
endfunction(ADD_HW NUMBER)
