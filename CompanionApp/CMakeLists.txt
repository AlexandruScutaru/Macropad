cmake_minimum_required(VERSION 3.19)

project(Macropad VERSION 0.0.1 LANGUAGES CXX)
set(TARGET_NAME "Companion")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QMLLS_NO_CMAKE_CALLS TRUE)

set(QT_DIR $ENV{QTDIR})
set(CMAKE_PREFIX_PATH ${QT_DIR})

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup()

add_subdirectory(qml/controls)

qt_add_executable(${TARGET_NAME}
    main.cpp
)

qt_policy(SET QTP0001 NEW)

set(qml_plain_files
    qml/Main.qml
    qml/Playground.qml
    qml/AppStackView.qml
    qml/NotConnectedView.qml
    qml/KeypadView.qml
    qml/SlidersView.qml
    qml/SettingsView.qml

    qml/components/ActionsList.qml
    qml/components/GradientSeparator.qml
    qml/components/NavigationBar.qml
    qml/components/NavigationBarModel.qml
    qml/components/Key.qml
    qml/components/KeyConfig.qml
    qml/components/KeypadLayers.qml
    qml/components/KeysGrid.qml
    qml/components/LayerPagination.qml
)

foreach(file_name ${qml_plain_files})
    get_filename_component(alias "${file_name}" NAME)
    set_source_files_properties(${file_name} PROPERTIES QT_RESOURCE_ALIAS ${alias})
endforeach()

set(qml_singletons
)
set_source_files_properties(${qml_singletons} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(${TARGET_NAME}
    URI MacropadCompanion
    VERSION 1.0

    QML_FILES
        ${qml_plain_files}

    SOURCES
        misc/CmdArgs.cpp
        misc/CmdArgs.h
        misc/DebugChecker.h
        misc/ListModel.cpp
        misc/ListModel.h
        misc/TaskRunner.h
        misc/Utils.cpp
        misc/Utils.h

        os/IPlatform.h

        hid/Device.cpp
        hid/Device.h
        hid/HidCommunicationThread.cpp
        hid/HidCommunicationThread.h
        hid/PotentiometersReader.cpp
        hid/PotentiometersReader.h

        theming/Theme.h
        theming/ThemeLoader.cpp
        theming/ThemeLoader.h

        tray/TrayIcon.cpp
        tray/TrayIcon.h

        AppSettings.cpp
        AppSettings.h
        Macropad.cpp
        Macropad.h
)

# devices
add_subdirectory(keypad)

# action handlers for the configurable keypad keys
add_subdirectory(action-handlers)

# unfortunately I have to add this to include path
# by using QML_ELEMENT, qt6 cmake will try to automatically register the type
# and the class in question needs to be accessible via include path without prefixes
target_include_directories(${TARGET_NAME} PUBLIC
    controllers
    controllers/settings
    keypad
    keypad/controller
    keypad/model
    theming
)

file(GLOB_RECURSE QML_SRC "*.qml")
source_group("QML Files" FILES ${QML_SRC})

qt_add_resources(${TARGET_NAME} "resources"
    PREFIX "/"
    FILES
        resources/back_arrow.svg
        resources/browser.svg
        resources/chevron_arrow.svg
        resources/close.svg
        resources/drag_point.svg
        resources/expand.svg
        resources/info.svg
        resources/keys.svg
        resources/looking_glass.svg
        resources/output_device.svg
        resources/rocket.svg
        resources/settings.svg
        resources/sliders.svg
        resources/system.svg
        resources/volume_down.svg
        resources/volume_toggle.svg
        resources/volume_up.svg

        resources/app_icon.png
        resources/not_connected_animation.gif
        resources/themes.json
)

target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QuickControls2)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Concurrent)

target_link_libraries(${TARGET_NAME} PRIVATE Controlsplugin)

# common configurations
set(NLOHMANN_JSON_INCLUDE "${CMAKE_SOURCE_DIR}/deps/common/nlohmann_json")
include_directories(SYSTEM ${NLOHMANN_JSON_INCLUDE})

# platform specific configurations
if(WIN32)
    set(HIDAPI_DIR "${CMAKE_SOURCE_DIR}/deps/windows/hidapi/cmake")
    find_package(HIDAPI REQUIRED)
    set(HIDAPI_INCLUDE "${CMAKE_SOURCE_DIR}/deps/windows/hidapi/include")
    include_directories(HIDAPI ${HIDAPI_INCLUDE})

    target_sources(${TARGET_NAME} PUBLIC
        os/windows/WindowsPlatform.cpp
        os/windows/WindowsPlatform.h
    )

    set_target_properties(${TARGET_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    target_link_libraries(${TARGET_NAME} PRIVATE User32)
    target_link_libraries(${TARGET_NAME} PRIVATE HIDAPI::hidapi)

    # post build action to copy dependencies in order to be able to debug from VSCode
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${QT_DIR}/bin/windeployqt.exe --qmldir ${PROJECT_SOURCE_DIR}/qml ${CMAKE_BINARY_DIR}/${TARGET_NAME}.exe
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}> COMMAND_EXPAND_LISTS
    )

    install(IMPORTED_RUNTIME_ARTIFACTS HIDAPI::hidapi)
elseif (LINUX)
    # on linux Qt6.9 needs WrapOpenGL (dnf install mesa-libGL-devel)

    # sudo dnf install hidapi hidapi-devel
    find_package(hidapi REQUIRED)

    # sudo dnf install pulseaudio-libs-devel
    find_package(PulseAudio REQUIRED)

    target_sources(${TARGET_NAME} PUBLIC
        os/linux/Audio.cpp
        os/linux/Audio.h
        os/linux/LinuxPlatform.cpp
        os/linux/LinuxPlatform.h
    )

    target_link_libraries(${TARGET_NAME} PRIVATE hidapi::hidapi)
    target_link_libraries(${TARGET_NAME} PRIVATE pulse)
endif()

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(TARGET ${TARGET_NAME}
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
