cmake_minimum_required(VERSION 3.16)  # CMake 3.16+ recommended for Qt6

project(tool-log VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# set(Qt6_DIR "/path/to/Qt/6.9.1/gcc_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CONTROLLER
    UiHandler.hpp
    UiHandler.cpp
    DataHandler.hpp
    DataHandler.cpp
)

set(MODEL
    FileHelper.hpp
    FileHelper.cpp
    ProcessHelper.hpp
    ProcessHelper.cpp
    NotificationHelper.hpp
    NotificationHelper.cpp
    LogHelper.hpp
    LogHelper.cpp
)

set(UTIL
    Log.hpp
    Log.cpp
    Logger.hpp
    Logger.cpp
    NotificationDialog.hpp
    NotificationDialog.cpp
    SettingDialog.hpp
    SettingDialog.cpp
    HighlightDelegate.hpp
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Settings.ui
    ${UTIL}
    ${MODEL}
    ${CONTROLLER}
)

qt_add_executable(tool-log
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    HighlightKey.hpp
    HighlightCell.hpp
    HighlightKey.cpp
    HighlightCell.cpp
)

target_link_libraries(tool-log PRIVATE Qt6::Widgets)

# Optional: Bundle settings for macOS/iOS
set_target_properties(tool-log PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tool-log
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(tool-log)
