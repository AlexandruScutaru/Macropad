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

qt_add_executable(appMacropadCompanion
    main.cpp
)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(appMacropadCompanion
    URI MacropadCompanion
    VERSION 1.0
    QML_FILES
        CButton.qml
        CIconButton.qml
        CSlider.qml
        CText.qml
        CTextField.qml
        CVerticalProgressBar.qml
        DevHelperView.qml
        DeviceCalibrationView.qml
        DeviceConnectView.qml
        DeviceDashboardView.qml
        DeviceListDelegate.qml
        DeviceSearchHeader.qml
        DeviceStackView.qml
        Main.qml
    SOURCES
        AudioOutputSwitcher.cpp
        AudioOutputSwitcher.h
        DevHelperController.cpp
        DevHelperController.h
        DevHelperModel.cpp
        DevHelperModel.h
        DeviceCalibrationModel.cpp
        DeviceCalibrationModel.h
        DeviceController.cpp
        DeviceController.h
        DeviceInfoModel.cpp
        DeviceInfoModel.h
        EventFilter.cpp
        EventFilter.h
        HidCommunicationThread.cpp
        HidCommunicationThread.h
        HidHelper.cpp
        HidHelper.h
        IPolicyConfig.h
        Macropad.cpp
        Macropad.h
        NativeEventFilter.cpp
        NativeEventFilter.h
        PotentiometersReader.cpp
        PotentiometersReader.h
        TrayIcon.cpp
        TrayIcon.h
        WinApiWrapper.cpp
        WinApiWrapper.h
)

file(GLOB_RECURSE QML_SRC "*.qml")
source_group("QML Files" FILES ${QML_SRC})

qt_add_resources(appMacropadCompanion "resources"
    PREFIX "/"
    FILES
        resources/app_icon.png
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
