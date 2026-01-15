cmake_minimum_required(VERSION 3.21)
project(LaunchDesk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
qt_standard_project_setup()

qt_add_executable(LaunchDesk
        main.cpp
        resources/resources.qrc
        application/ui/LaunchDeskWindow.cpp
        application/ui/LaunchDeskWindow.h
        application/controllers/DockController.cpp
        application/controllers/DockController.h
        application/platform/WinHotkeyFilter.cpp
        application/platform/WinHotkeyFilter.h
        application/controllers/WindowsConsole.cpp
        application/controllers/WindowsConsole.h
        application/controllers/DebugAppConsole.cpp
        application/controllers/DebugAppConsole.h
        application/controllers/debug.cpp
        application/controllers/debug.h
        application/controllers/ProfileController.cpp
        application/controllers/ProfileController.h
        application/controllers/ProfilePage.cpp
        application/controllers/ProfilePage.h
        application/controllers/ProfileData.h
)

target_include_directories(LaunchDesk PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        application/ui
        application/ui/windows
        application/ui/helpers
        ${CMAKE_CURRENT_SOURCE_DIR}/core
)

target_link_libraries(LaunchDesk PRIVATE
        Qt6::Widgets
        Qt6::Svg
)

# --------------------------------------------------
# Windows specifics
# --------------------------------------------------

if (WIN32)
    target_sources(LaunchDesk PRIVATE resources/appicon.rc)
    target_link_libraries(LaunchDesk PRIVATE shell32 ole32 uuid)
endif()

if (MINGW)
    target_link_options(LaunchDesk PRIVATE -Wl,--subsystem,windows)
endif()

# --------------------------------------------------
# Portable release build (windeployqt)
# --------------------------------------------------

set(QT_BIN_DIR     "C:/Qt/6.10.0/mingw_64/bin")
set(MINGW_BIN_DIR  "C:/Qt/Tools/mingw1310_64/bin")
set(WINDEPLOYQT    "${QT_BIN_DIR}/windeployqt.exe")
set(RELEASE_DIR    "${CMAKE_BINARY_DIR}/release/LaunchDesk")

add_custom_target(build_application
        COMMENT "Building portable LaunchDesk application"
)

add_custom_command(TARGET build_application POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:LaunchDesk>"
        "${RELEASE_DIR}"

        COMMAND "${WINDEPLOYQT}"
        --no-translations
        --no-system-d3d-compiler
        --no-opengl-sw
        "${RELEASE_DIR}/LaunchDesk.exe"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MINGW_BIN_DIR}/libstdc++-6.dll"
        "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        "${RELEASE_DIR}"
)

add_dependencies(build_application LaunchDesk)
