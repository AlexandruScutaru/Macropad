cmake_minimum_required(VERSION 3.19)

project(Macropad VERSION 0.0.1 LANGUAGES CXX)
set(TARGET_NAME "MacropadCompanion")

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

set(HIDAPI_DIR ./deps/hidapi/cmake)
find_package(HIDAPI REQUIRED)
set(HIDAPI_INCLUDE ./deps/hidapi/include)
include_directories(HIDAPI ${HIDAPI_INCLUDE})

qt_standard_project_setup()

add_subdirectory(qml/controls)

qt_add_executable(${TARGET_NAME}
    main.cpp
)

qt_policy(SET QTP0001 NEW)

set_source_files_properties(qml/DeviceDashboardView.qml           PROPERTIES QT_RESOURCE_ALIAS DeviceDashboardView.qml)
set_source_files_properties(qml/DeviceStackView.qml               PROPERTIES QT_RESOURCE_ALIAS DeviceStackView.qml)

set_source_files_properties(qml/connecting/DeviceConnectView.qml  PROPERTIES QT_RESOURCE_ALIAS DeviceConnectView.qml)
set_source_files_properties(qml/connecting/DeviceListDelegate.qml PROPERTIES QT_RESOURCE_ALIAS DeviceListDelegate.qml)
set_source_files_properties(qml/connecting/DeviceSearchHeader.qml PROPERTIES QT_RESOURCE_ALIAS DeviceSearchHeader.qml)

set_source_files_properties(qml/settings/DeviceSettings.qml       PROPERTIES QT_RESOURCE_ALIAS DeviceSettings.qml)
set_source_files_properties(qml/settings/HotkeysSettings.qml      PROPERTIES QT_RESOURCE_ALIAS HotkeysSettings.qml)
set_source_files_properties(qml/settings/SettingsView.qml         PROPERTIES QT_RESOURCE_ALIAS SettingsView.qml)
set_source_files_properties(qml/settings/SlidersSettings.qml      PROPERTIES QT_RESOURCE_ALIAS SlidersSettings.qml)

set_source_files_properties(qml/Main.qml                          PROPERTIES QT_RESOURCE_ALIAS Main.qml)

qt_add_qml_module(${TARGET_NAME}
    URI MacropadCompanion
    VERSION 1.0

    QML_FILES
        qml/DeviceDashboardView.qml
        qml/DeviceStackView.qml

        qml/connecting/DeviceConnectView.qml
        qml/connecting/DeviceListDelegate.qml
        qml/connecting/DeviceSearchHeader.qml

        qml/settings/DeviceSettings.qml
        qml/settings/HotkeysSettings.qml
        qml/settings/SettingsView.qml
        qml/settings/SlidersSettings.qml

        qml/Main.qml

    SOURCES
        audio/AudioOutputSwitcher.cpp
        audio/AudioOutputSwitcher.h

        controllers/MainController.cpp
        controllers/MainController.h
        controllers/settings/DeviceSettingsController.cpp
        controllers/settings/DeviceSettingsController.h
        controllers/settings/HotkeyActions.cpp
        controllers/settings/HotkeyActions.h
        controllers/settings/HotkeysSettingsController.cpp
        controllers/settings/HotkeysSettingsController.h
        controllers/settings/SlidersSettingsController.cpp
        controllers/settings/SlidersSettingsController.h

        misc/DebugChecker.h

        models/DeviceInfoModel.cpp
        models/DeviceInfoModel.h
        models/SlidersModel.cpp
        models/SlidersModel.h
        models/settings/HotkeysSettingsModel.cpp
        models/settings/HotkeysSettingsModel.h
        models/settings/SlidersSettingsModel.cpp
        models/settings/SlidersSettingsModel.h

        hid/HidCommunicationThread.cpp
        hid/HidCommunicationThread.h
        hid/HidHelper.cpp
        hid/HidHelper.h
        hid/PotentiometersReader.cpp
        hid/PotentiometersReader.h

        tray/TrayIcon.cpp
        tray/TrayIcon.h

        Config.cpp
        Config.h
        Macropad.cpp
        Macropad.h
)

# unfortunately I have to add this to include path
# by using QML_ELEMENT, qt6 cmake will try to automatically register the type
# and the class in question needs to be accessible via include path without prefixes
target_include_directories(${TARGET_NAME} PUBLIC
    controllers
    controllers/settings
)

if (WIN32)
    target_sources(${TARGET_NAME} PUBLIC
        os/windows/IPolicyConfig.h
        os/windows/NativeEventFilter.cpp
        os/windows/NativeEventFilter.h
        os/windows/WinApiWrapper.cpp
        os/windows/WinApiWrapper.h
    )
endif()

if (LINUX)
    target_sources(${TARGET_NAME} PUBLIC
    )
endif()

file(GLOB_RECURSE QML_SRC "*.qml")
source_group("QML Files" FILES ${QML_SRC})

qt_add_resources(${TARGET_NAME} "resources"
    PREFIX "/"
    FILES
        resources/app_icon.png
        resources/back_arrow_icon.png
        resources/close_icon.png
        resources/info_icon.png
        resources/search_icon.png
        resources/sliders_icon.png
)

set_target_properties(${TARGET_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QuickControls2)
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets)

target_link_libraries(${TARGET_NAME} PRIVATE Controlsplugin)

target_link_libraries(${TARGET_NAME} PRIVATE HIDAPI::hidapi)
target_link_libraries(${TARGET_NAME} PRIVATE User32)

set(BUILD_DIR ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

# post build action to copy dependencies in order to be able to debug from VSCode
if(WIN32)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${QT_DIR}/bin/windeployqt.exe --qmldir ${PROJECT_SOURCE_DIR}/qml ${BUILD_DIR}/${TARGET_NAME}.exe
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}> COMMAND_EXPAND_LISTS
    )
endif()

set(DEPLOY_DIR ${BUILD_DIR}/deploy)
set(CMAKE_INSTALL_PREFIX ${DEPLOY_DIR})

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(IMPORTED_RUNTIME_ARTIFACTS HIDAPI::hidapi)

qt_generate_deploy_qml_app_script(TARGET ${TARGET_NAME}
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
