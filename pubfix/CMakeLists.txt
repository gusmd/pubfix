cmake_minimum_required(VERSION 3.2)
project(pubfix)

add_executable(pubfix main.cpp MainWindow.h MainWindow.cpp ../QDarkStyleSheet/qdarkstyle/style.qrc)

set_target_properties(pubfix
    PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

find_package(Qt5Widgets)

target_link_libraries(pubfix Qt5::Widgets QHotkey)

install(TARGETS pubfix
        ARCHIVE
        RUNTIME DESTINATION .
        COMPONENT pubfix
)