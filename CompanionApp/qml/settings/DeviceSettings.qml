import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0
import MacropadCompanion 1.0


Item {
    DeviceSettingsController {
        id: deviceController
    }

    Component.onCompleted: {
        deviceController.init(controller.getConfig());
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        CText {
            Layout.fillWidth: true

            label: qsTr("Connected device")
            fontSize: 16
            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
        }

        RowLayout {
            spacing: 8

            Rectangle {
                Layout.fillWidth: true
                height: forgetDeviceButton.height

                color: "#222222"
                radius: 8

                CText {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4

                    label: deviceController.devicePath
                    toolTipText: deviceController.devicePath
                    fontSize: 12

                    hAlign: Text.AlignLeft
                    vAlign: Text.AlignVCenter
                    elide: Qt.ElideRight
                }
            }

            CButton {
                id: forgetDeviceButton

                Layout.alignment: Qt.AlignRight

                label: qsTr("Forget device")
                toolTipText: qsTr("Disconnect and forget this device")

                onButtonClicked: {
                    deviceController.forgetDeviceClicked();
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
