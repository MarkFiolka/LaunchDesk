cmake_minimum_required(VERSION 3.21)
project(LaunchDesk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets Svg REQUIRED)
qt_standard_project_setup()

qt_add_executable(LaunchDesk
        main.cpp

        ui/windows/Guide.cpp
        ui/windows/Guide.h
        ui/windows/LaunchDesk.cpp
        ui/windows/LaunchDesk.h
        ui/windows/Welcome.cpp
        ui/windows/Welcome.h

        ui/helpers/UIHelpers.cpp
        ui/helpers/UIHelpers.h

        core/ContentConfig.h
        core/ContentStorage.cpp
        core/ContentStorage.h
        core/CommandTemplate.cpp
        core/CommandTemplate.h
        core/WindowPlacement.cpp
        core/WindowPlacement.h
        core/ContentRow.cpp
        core/ContentRow.h

        core/ConfigManager.cpp
        core/ConfigManager.h
        core/ResourcePaths.cpp
        core/ResourcePaths.h
        core/AppPickerWindow.cpp
        core/AppPickerWindow.h
)

target_include_directories(LaunchDesk PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/windows
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/helpers
)

if (WIN32)
    target_sources(LaunchDesk PRIVATE resources/appicon.rc)
endif()

if (MINGW)
    target_link_options(LaunchDesk PRIVATE -Wl,--subsystem,console)
endif()

qt_add_resources(LaunchDeskResources resources/resources.qrc)
target_sources(LaunchDesk PRIVATE ${LaunchDeskResources})

target_link_libraries(LaunchDesk PRIVATE Qt6::Widgets Qt6::Svg)

if (WIN32)
    target_link_libraries(LaunchDesk PRIVATE shell32 ole32 uuid)
endif()

set(QT_BIN_DIR "C:/Qt/6.10.0/mingw_64/bin")
set(QT_PLUGIN_DIR "C:/Qt/6.10.0/mingw_64/plugins")
set(MINGW_BIN_DIR "C:/Qt/Tools/mingw1310_64/bin")

add_custom_command(TARGET LaunchDesk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying Qt DLLs..."

        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_BIN_DIR}/Qt6Core.dll"            "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_BIN_DIR}/Qt6Gui.dll"             "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_BIN_DIR}/Qt6Widgets.dll"         "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_BIN_DIR}/Qt6Svg.dll"             "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_BIN_DIR}/Qt6SvgWidgets.dll"      "$<TARGET_FILE_DIR:LaunchDesk>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_BIN_DIR}/libstdc++-6.dll"     "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"  "$<TARGET_FILE_DIR:LaunchDesk>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MINGW_BIN_DIR}/libwinpthread-1.dll" "$<TARGET_FILE_DIR:LaunchDesk>"

        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LaunchDesk>/imageformats"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_PLUGIN_DIR}/imageformats/qsvg.dll"
        "$<TARGET_FILE_DIR:LaunchDesk>/imageformats"
)

add_custom_command(TARGET LaunchDesk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LaunchDesk>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_PLUGIN_DIR}/platforms/qwindows.dll"
        "$<TARGET_FILE_DIR:LaunchDesk>/platforms"
)
