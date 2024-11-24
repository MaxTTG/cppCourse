project(HW6 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(HEADERS
    hw6.hpp

    Matrix.hpp
)

set(SOURCES
    hw6.cpp
)

add_library(HW6_LIB ${HEADERS} ${SOURCES})

set_target_properties(HW6_LIB PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
