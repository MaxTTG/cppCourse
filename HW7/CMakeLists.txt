project(HW7 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(HEADERS
    hw7.hpp
    BulkCommandHandler.hpp
    Logger.hpp
)

set(SOURCES
    hw7.cpp
    BulkCommandHandler.cpp
    Logger.cpp
)

add_library(HW7_LIB ${HEADERS} ${SOURCES})

set_target_properties(HW7_LIB PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
