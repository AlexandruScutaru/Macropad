cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MODULE_NAME "Controls")

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_policy(SET QTP0001 NEW)

qt_add_library(${MODULE_NAME} STATIC)
qt_add_qml_module(${MODULE_NAME}
    URI ${MODULE_NAME}
    VERSION 1.0
    QML_FILES
        CBusyIndicator.qml
        CButton.qml
        CCollapseableHeader.qml
        CDragableItem.qml
        CIcon.qml
        CIconButton.qml
        CSlider.qml
        CTabButton.qml
        CText.qml
        CTextField.qml
        CVerticalProgressBar.qml
        CVerticalSlider.qml
    SOURCES
)
