cmake_minimum_required(VERSION 3.16)  # CMake 3.16+ recommended for Qt6

project(tool-log VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# set(Qt6_DIR "/home/truongnguyen/Qt/6.9.1/gcc_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/forms
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(FORMS
    src/forms/Settings.ui
    src/forms/mainwindow.ui
)
    
set(UI

    src/ui/NotificationDialog.hpp
    src/ui/NotificationDialog.cpp
    src/ui/QPushButtonDefault.h
    src/ui/QLineEditDefault.h
    src/ui/SettingDialog.hpp
    src/ui/SettingDialog.cpp
    src/ui/HighlightCell.hpp
    src/ui/HighlightDelegate.hpp
    src/ui/HighlightKey.hpp
)

set(CORE
    src/core/UiHandler.hpp
    src/core/UiHandler.cpp
    src/core/DataHandler.hpp
    src/core/DataHandler.cpp
)

set(UTIL
    src/utils/Logger.hpp
    src/utils/UtilHelper.hpp
    src/utils/Constant.hpp
    src/utils/Property.hpp
    src/utils/Setting.hpp
    src/utils/Log.hpp
)

set(CONTROL
    src/control/LogHelper.cpp
    src/control/LogHelper.hpp
    src/control/FileHelper.hpp
    src/control/FileHelper.cpp
    src/control/ProcessHelper.hpp
    src/control/PropertyHelper.hpp
    src/control/SettingHelper.hpp
    src/control/SettingHelper.cpp
    src/control/ProcessHelper.cpp
    src/control/PropertyHelper.cpp
)

set(PROJECT_SOURCES
    main.cpp
    ${CONTROL}
    ${CORE}
    ${FORMS}
    ${UI}
    ${UTIL}
)

qt_add_executable(tool-log
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    src/mainwindow.h
    src/mainwindow.cpp
    resource.qrc
)

target_link_libraries(tool-log PRIVATE Qt6::Widgets)

# Add include directories for each component
target_include_directories(tool-log PRIVATE
    ${CMAKE_SOURCE_DIR}/src/control
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/forms
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/
)

# Optional: Bundle settings for macOS/iOS
set_target_properties(tool-log PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tool-log
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(tool-log)
