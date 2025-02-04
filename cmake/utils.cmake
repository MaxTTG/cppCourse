function(ADD_HW NUMBER)
    add_subdirectory(HW${NUMBER})
    target_link_libraries(${PROJECT_NAME} HW${NUMBER}_LIB)

    if (MSVC)
        target_compile_options(HW${NUMBER}_LIB PRIVATE
            /W4
        )
    else ()
        target_compile_options(HW${NUMBER}_LIB PRIVATE
            -Wall -Wextra -pedantic -Werror -fPIC
        )
    endif()
endfunction(ADD_HW NUMBER)
