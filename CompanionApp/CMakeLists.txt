cmake_minimum_required(VERSION 3.16)

project(MacropadCompanion VERSION 0.1 LANGUAGES CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${BUILD_DIRECTORY}/output/ CACHE PATH "Custom Macropad install directory" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
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

qt_add_executable(appMacropadCompanion
    main.cpp
)

qt_policy(SET QTP0001 NEW)

set_source_files_properties(qml/DevHelperView.qml                 PROPERTIES QT_RESOURCE_ALIAS DevHelperView.qml)
set_source_files_properties(qml/DeviceCalibrationView.qml         PROPERTIES QT_RESOURCE_ALIAS DeviceCalibrationView.qml)
set_source_files_properties(qml/DeviceDashboardView.qml           PROPERTIES QT_RESOURCE_ALIAS DeviceDashboardView.qml)
set_source_files_properties(qml/DeviceStackView.qml               PROPERTIES QT_RESOURCE_ALIAS DeviceStackView.qml)

set_source_files_properties(qml/connecting/DeviceConnectView.qml  PROPERTIES QT_RESOURCE_ALIAS connecting/DeviceConnectView.qml)
set_source_files_properties(qml/connecting/DeviceListDelegate.qml PROPERTIES QT_RESOURCE_ALIAS connecting/DeviceListDelegate.qml)
set_source_files_properties(qml/connecting/DeviceSearchHeader.qml PROPERTIES QT_RESOURCE_ALIAS connecting/DeviceSearchHeader.qml)

set_source_files_properties(qml/settings/DeviceSettingsView.qml   PROPERTIES QT_RESOURCE_ALIAS settings/DeviceSettingsView.qml)
set_source_files_properties(qml/settings/Hotkeys.qml              PROPERTIES QT_RESOURCE_ALIAS settings/Hotkeys.qml)
set_source_files_properties(qml/settings/Sliders.qml              PROPERTIES QT_RESOURCE_ALIAS settings/Sliders.qml)
set_source_files_properties(qml/settings/Device.qml               PROPERTIES QT_RESOURCE_ALIAS settings/Device.qml)

set_source_files_properties(qml/Main.qml PROPERTIES QT_RESOURCE_ALIAS Main.qml)

qt_add_qml_module(appMacropadCompanion
    URI MacropadCompanion
    VERSION 1.0
    QML_FILES
        qml/DevHelperView.qml
        qml/DeviceCalibrationView.qml
        qml/DeviceDashboardView.qml
        qml/DeviceStackView.qml

        qml/connecting/DeviceConnectView.qml
        qml/connecting/DeviceListDelegate.qml
        qml/connecting/DeviceSearchHeader.qml

        qml/settings/DeviceSettingsView.qml
        qml/settings/Hotkeys.qml
        qml/settings/Sliders.qml
        qml/settings/Device.qml

        qml/Main.qml
    SOURCES
        audio/AudioOutputSwitcher.cpp
        audio/AudioOutputSwitcher.h

        controllers/DevHelperController.cpp
        controllers/DevHelperController.h
        controllers/DeviceController.cpp
        controllers/DeviceController.h

        misc/DebugChecker.h

        models/DevHelperModel.cpp
        models/DevHelperModel.h
        models/DeviceCalibrationModel.cpp
        models/DeviceCalibrationModel.h
        models/DeviceInfoModel.cpp
        models/DeviceInfoModel.h

        hid/HidCommunicationThread.cpp
        hid/HidCommunicationThread.h
        hid/HidHelper.cpp
        hid/HidHelper.h
        hid/PotentiometersReader.cpp
        hid/PotentiometersReader.h

        tray/TrayIcon.cpp
        tray/TrayIcon.h

        Macropad.cpp
        Macropad.h
)

# unfortunately I have to add this to include path
# by using QML_ELEMENT, qt6 cmake will try yo automatically register the type
# and the class in question needs to be accessible via include path without prefixes
target_include_directories(appMacropadCompanion PUBLIC
    controllers
)

if (WIN32)
    target_sources(appMacropadCompanion PUBLIC
        os/windows/IPolicyConfig.h
        os/windows/NativeEventFilter.cpp
        os/windows/NativeEventFilter.h
        os/windows/WinApiWrapper.cpp
        os/windows/WinApiWrapper.h
    )
endif()

if (LINUX)
    target_sources(appMacropadCompanion PUBLIC
    )
endif()

file(GLOB_RECURSE QML_SRC "*.qml")
source_group("QML Files" FILES ${QML_SRC})

qt_add_resources(appMacropadCompanion "resources"
    PREFIX "/"
    FILES
        resources/app_icon.png
        resources/back_arrow_icon.png
        resources/close_icon.png
        resources/info_icon.png
        resources/search_icon.png
        resources/sliders_icon.png
)

set_target_properties(appMacropadCompanion PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMacropadCompanion PRIVATE Qt6::Quick)
target_link_libraries(appMacropadCompanion PRIVATE Qt6::QuickControls2)
target_link_libraries(appMacropadCompanion PRIVATE Qt6::Widgets)

target_link_libraries(appMacropadCompanion PRIVATE HIDAPI::hidapi)
target_link_libraries(appMacropadCompanion PRIVATE User32)

include(GNUInstallDirs)
install(TARGETS appMacropadCompanion
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(IMPORTED_RUNTIME_ARTIFACTS HIDAPI::hidapi)

qt_generate_deploy_qml_app_script(TARGET appMacropadCompanion
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
