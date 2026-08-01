import QtQuick

import Controls

ListModel {
    id: model

    readonly property string keypadTabName: qsTr("Keypad")
    readonly property string slidersTabName: qsTr("Sliders")
    readonly property string settingsTabName: qsTr("Settings")

    property bool keypadTabEnabled: false
    property bool slidersTabEnabled: false

    onKeypadTabEnabledChanged: {
        if (model.count) {
            model.setProperty(0, "tabEnabled", keypadTabEnabled);
        }
    }

    onSlidersTabEnabledChanged: {
        if (model.count) {
            model.setProperty(1, "tabEnabled", slidersTabEnabled);
        }
    }

    Component.onCompleted: {
        model.append([
            {
                type: "button",
                name: keypadTabName,
                tabEnabled: keypadTabEnabled,
                tabCheckable: true,
                iconSource: "qrc:///resources/icons/keys.svg",
                animation: CIcon.AnimationType.Scale
            },
            {
                type: "button",
                name: slidersTabName,
                tabEnabled: slidersTabEnabled,
                tabCheckable: true,
                iconSource: "qrc:///resources/icons/sliders.svg",
                animation: CIcon.AnimationType.Scale
            },
            {
                type: "spacer",
            },
            {
                type: "button",
                name: settingsTabName,
                tabEnabled: true,
                tabCheckable: false,
                iconSource: "qrc:///resources/icons/settings.svg",
                animation: CIcon.AnimationType.Rotate
            }
        ]);
    }
}
