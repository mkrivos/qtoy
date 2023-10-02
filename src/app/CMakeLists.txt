cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(qtoy
    main.cpp 
    mainwindow.cpp
    centralwidget.cpp
    mainwindow.ui
    resources.qrc
)

target_link_libraries(qtoy PRIVATE Qt6::Widgets shaders)

set_target_properties(qtoy PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)