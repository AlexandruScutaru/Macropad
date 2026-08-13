import QtQuick
import QtQuick.Controls.Basic


Popup {
    id: popup

    required property SettingsController controller

    parent: Overlay.overlay
    anchors.centerIn: parent
    width: parent.width * 0.7
    height: parent.height * 0.8

    focus: opened
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "transparent"
    }

    Overlay.modal: Rectangle {
        color: Theme.backgroundBackdrop
    }

    onAboutToHide: {
        settingsLoader.setSource("");
    }

    contentItem: Item {
        anchors.fill: parent

        Loader {
            id: settingsLoader

            anchors.fill: parent

            active: popup.opened

            onActiveChanged: {
                if (!active) {
                    return;
                }

                setSource("/qt/qml/MacropadCompanion/SettingsView.qml", {
                    controller: popup.controller
                });
            }
        }

        Connections {
            target: settingsLoader.item
            function onCloseRequested() {
                popup.close();
            }
        }
    }
}
