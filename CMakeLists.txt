cmake_minimum_required(VERSION 3.16)

project(tripleytz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()

set(TS_FILES
    tripleytz_en_US.ts
)

set(XPM_FILES
    src/ace.xpm
    src/two.xpm
    src/three.xpm
    src/four.xpm
    src/five.xpm
    src/six.xpm
)

set(PROJECT_SOURCES
    src/dice.h
    src/gamescorer.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/score.h
    src/scorecolumn.h
    src/scorerow.h
    ${XPM_FILES}
    ${TS_FILES}
)

qt_add_executable(tripleytz
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(tripleytz PRIVATE Qt6::Widgets)

set_target_properties(tripleytz PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER tripleytz.jeffbi.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS tripleytz
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_finalize_executable(tripleytz)
