project(HW5 VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(HEADERS
    hw5.hpp

    Screen.hpp
    Document.hpp
    MainWindow.hpp

    Tools/IBar.hpp
    Tools/ToolBar.hpp

    Tools/Primitives/Color.hpp
    Tools/Primitives/IPrimitive.cpp
    Tools/Primitives/Line.cpp
    Tools/Primitives/Oval.cpp
    Tools/Primitives/Rectangle.cpp
)

set(SOURCES
    hw5.cpp

    Screen.cpp
    Document.cpp
    MainWindow.cpp

    Tools/IBar.cpp
    Tools/ToolBar.cpp

    Tools/Primitives/Color.cpp
    Tools/Primitives/IPrimitive.hpp
    Tools/Primitives/Line.hpp
    Tools/Primitives/Oval.hpp
    Tools/Primitives/Rectangle.hpp
)

add_library(HW5_LIB ${HEADERS} ${SOURCES})

set_target_properties(HW5_LIB PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
