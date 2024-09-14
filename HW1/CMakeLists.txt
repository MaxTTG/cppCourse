project(HW1 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

configure_file(version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp)

set(HEADERS hw1.hpp)
set(SOURCES hw1.cpp)

add_library(HW1_LIB ${HEADERS} ${SOURCES})