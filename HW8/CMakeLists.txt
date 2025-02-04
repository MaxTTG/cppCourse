project(HW8 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

find_package(Boost REQUIRED COMPONENTS program_options filesystem regex)

set(HEADERS
    hw8.hpp
    FileReader.hpp
    FileInfo.hpp
    Options.hpp
)

set(SOURCES
    hw8.cpp
)

add_library(HW8_LIB ${HEADERS} ${SOURCES})

target_include_directories(HW8_LIB PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(HW8_LIB PUBLIC Boost::program_options Boost::filesystem Boost::regex)

set_target_properties(HW8_LIB PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
