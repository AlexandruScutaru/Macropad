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
        model.setProperty(0, "tabEnabled", keypadTabEnabled);
    }

    onSlidersTabEnabledChanged: {
        model.setProperty(1, "tabEnabled", slidersTabEnabled);
    }

    Component.onCompleted: {
        model.append([
            {
                type: "button",
                name: keypadTabName,
                tabEnabled: keypadTabEnabled,
                tabCheckable: true,
                iconSource: "qrc:///resources/keys_icon.svg",
                animation: CIcon.AnimationType.Scale
            },
            {
                type: "button",
                name: slidersTabName,
                tabEnabled: slidersTabEnabled,
                tabCheckable: true,
                iconSource: "qrc:///resources/sliders_icon.svg",
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
                iconSource: "qrc:///resources/settings_icon.svg",
                animation: CIcon.AnimationType.Rotate
            }
        ]);
    }
}
