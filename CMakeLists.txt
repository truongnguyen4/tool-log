cmake_minimum_required(VERSION 3.16)  # CMake 3.16+ recommended for Qt6

project(tool-log VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(Qt6_DIR "/path/to/Qt/6.9.1/gcc_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Log.hpp
        Log.cpp
        FileLogHelper.hpp
        FileLogHelper.cpp
        Logger.hpp
        Logger.cpp
        FilterLogHelper.hpp
        FilterLogHelper.cpp
        UiHandler.hpp
        UiHandler.cpp
        DataHandler.hpp
        DataHandler.cpp
        HighlightDelegate.hpp
        HighlightDelegate.cpp
        NotificationDialog.hpp
        NotificationDialog.cpp
        Settings.ui
        SettingDialog.hpp
        SettingDialog.cpp
        ProcessHandler.hpp
        ProcessHandler.cpp
        NotificationManager.hpp
        NotificationManager.cpp
)

qt_add_executable(tool-log
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}

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
