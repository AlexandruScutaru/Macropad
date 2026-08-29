cmake_minimum_required(VERSION 3.19)

project(Macropad VERSION 0.0.1 LANGUAGES CXX)
set(TARGET_NAME "Companion")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QMLLS_NO_CMAKE_CALLS ON)

set(QT_DIR $ENV{QTDIR})
set(CMAKE_PREFIX_PATH ${QT_DIR})

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup()

qt_add_executable(${TARGET_NAME}
    main.cpp
)

qt_policy(SET QTP0001 NEW)

# custom qml components library plugin
add_subdirectory(qml-components)

# variable to collect all qml plain files into
set(QML_PLAIN_FILES
)
macro(append_qml_files)
    foreach(file ${ARGN})
        list(APPEND QML_PLAIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    endforeach()

    if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(QML_PLAIN_FILES "${QML_PLAIN_FILES}" PARENT_SCOPE)
    endif()
endmacro()

add_subdirectory(app)
add_subdirectory(action-handlers)
add_subdirectory(module/keypad)
add_subdirectory(module/sliders)
add_subdirectory(settings)

# adding qml files sources to the resource
foreach(file_name ${QML_PLAIN_FILES})
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
        ${QML_PLAIN_FILES}

    SOURCES
        helpers/ListModel.cpp
        helpers/ListModel.h
        helpers/String.cpp
        helpers/String.h
        helpers/TaskRunner.h

        hid/Device.cpp
        hid/Device.h
        hid/HidCommunicationThread.cpp
        hid/HidCommunicationThread.h
        hid/PotentiometersReader.cpp
        hid/PotentiometersReader.h

        services/HidDeviceService.cpp
        services/HidDeviceService.h
        services/SettingsService.cpp
        services/SettingsService.h

        os/IPlatform.h
)

# unfortunately I have to add this to include path
# by using QML_ELEMENT, qt6 cmake will try to automatically register the type
# and the class in question needs to be accessible via include path without prefixes
target_include_directories(${TARGET_NAME} PUBLIC
    app/controller
    app/theming

    module/keypad/controller
    module/keypad/model
    module/keypad/service

    settings/controller
    settings/model
)

file(GLOB_RECURSE QML_SRC "*.qml")
source_group("QML Files" FILES ${QML_SRC})

qt_add_resources(${TARGET_NAME} "resources"
    PREFIX "/"
    FILES
        resources/app_icon.png
        resources/themes.json

        resources/icons/back_arrow.svg
        resources/icons/browser.svg
        resources/icons/chevron_arrow.svg
        resources/icons/close.svg
        resources/icons/drag_point.svg
        resources/icons/expand.svg
        resources/icons/folder.svg
        resources/icons/info.svg
        resources/icons/keys.svg
        resources/icons/looking_glass.svg
        resources/icons/output_device.svg
        resources/icons/rocket.svg
        resources/icons/settings.svg
        resources/icons/sliders.svg
        resources/icons/system.svg
        resources/icons/volume_down.svg
        resources/icons/volume_toggle.svg
        resources/icons/volume_up.svg

        resources/anims/not_connected_animation_dark.gif
        resources/anims/not_connected_animation_light.gif
)

target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QuickControls2)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Concurrent)

target_link_libraries(${TARGET_NAME} PRIVATE Componentsplugin)

# common configurations
set(NLOHMANN_JSON_INCLUDE "${CMAKE_SOURCE_DIR}/deps/common/nlohmann_json")
include_directories(SYSTEM ${NLOHMANN_JSON_INCLUDE})

# platform specific configurations
if(WIN32)
    add_subdirectory(os/windows)

    set(HIDAPI_DIR "${CMAKE_SOURCE_DIR}/deps/windows/hidapi/cmake")
    find_package(HIDAPI REQUIRED)
    set(HIDAPI_INCLUDE "${CMAKE_SOURCE_DIR}/deps/windows/hidapi/include")
    include_directories(HIDAPI ${HIDAPI_INCLUDE})

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
    add_subdirectory(os/linux)

    # on linux Qt6.9 needs WrapOpenGL (dnf install mesa-libGL-devel)

    # sudo dnf install hidapi hidapi-devel
    find_package(hidapi REQUIRED)
    # sudo dnf install pulseaudio-libs-devel
    find_package(PulseAudio REQUIRED)

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
