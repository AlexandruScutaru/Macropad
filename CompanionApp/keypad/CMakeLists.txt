cmake_minimum_required(VERSION 3.19)

target_sources(${TARGET_NAME} PRIVATE
    KeypadModule.cpp
    KeypadModule.h
    KeypadTypes.h

    service/KeypadService.cpp
    service/KeypadService.h

    controller/ActionConfigController.cpp
    controller/ActionConfigController.h
    controller/AvailableActionsController.cpp
    controller/AvailableActionsController.h
    controller/KeypadController.cpp
    controller/KeypadController.h

    model/ActionConfigListModel.cpp
    model/ActionConfigListModel.h
    model/ActionsListModel.cpp
    model/ActionsListModel.h
    model/ActionSectionsListModel.cpp
    model/ActionSectionsListModel.h
    model/KeysListModel.cpp
    model/KeysListModel.h
)
