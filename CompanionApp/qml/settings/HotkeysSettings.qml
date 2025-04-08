import QtQuick 2.15
import QtQuick.Layouts

import Controls 1.0
import MacropadCompanion 1.0

Item {
    HotkeysSettingsController {
        id: hotkeysController
    }

    Component.onCompleted: {
        hotkeysController.init(mainController.getConfig());

        // in order to remap hotkeys, they need to be temporarily unregistered
        // the OS will prevent registered hotkeys to be processed by normal qt event system
        hotkeysController.unregisterHotkeys();
    }

    Component.onDestruction: {
        hotkeysController.restoreHotkeys();
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: 8

        RowLayout {
            spacing: 8

            CText {
                Layout.preferredWidth: mainLayout.width * 0.5
                Layout.alignment: Qt.AlignLeft

                label: qsTr("Action")
                fontSize: 16
                hAlign: Text.AlignLeft
                vAlign: Text.AlignVCenter
            }

            CText {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft

                label: qsTr("Key")
                fontSize: 16
                hAlign: Text.AlignLeft
                vAlign: Text.AlignVCenter
            }
        }

        Rectangle {
            Layout.fillWidth: true

            height: 1
            color: "#d9e7cb";
        }

        Repeater {
            model: hotkeysController.getModel()

            RowLayout {
                spacing: 8

                CText {
                    Layout.preferredWidth: mainLayout.width * 0.5

                    Layout.alignment: Qt.AlignLeft
                    label: model.name
                    fontSize: 14
                    hAlign: Text.AlignLeft
                    vAlign: Text.AlignVCenter
                }

                CKeyBinder {
                    key: model.key
                    toolTipText: qsTr("Record the desired key to trigger the action")

                    onKeyBindRecorded: (key) => {
                        hotkeysController.setActionKeyMapping(model.id, key);
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        CButton {
            id: saveButton
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 8

            label: qsTr("Save")
            toolTipText: qsTr("Save changes")
            enabled: hotkeysController.hasPendingChanges

            onButtonClicked: {
                hotkeysController.saveChanges();
            }
        }
    }
}
