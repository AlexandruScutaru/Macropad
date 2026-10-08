target_sources(${TARGET_NAME} PRIVATE
    controller/SettingsController.cpp
    controller/SettingsController.h

    model/TabsListModel.cpp
    model/TabsListModel.h
    model/ThemesListModel.cpp
    model/ThemesListModel.h

    service/SettingsService.cpp
    service/SettingsService.h
)

append_qml_files(
    view/Appearance.qml
    view/SettingsPopup.qml
    view/SettingsView.qml
    view/ThemePreview.qml
    view/ThemeSelector.qml
)
