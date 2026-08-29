target_sources(${TARGET_NAME} PRIVATE
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

    service/KeypadService.cpp
    service/KeypadService.h
    service/KeypadTypes.h
)

append_qml_files(
    view/KeypadView.qml
    view/ActionsList.qml
    view/Key.qml
    view/KeyConfig.qml
    view/KeypadLayers.qml
    view/KeysGrid.qml
    view/LayerPagination.qml
)
