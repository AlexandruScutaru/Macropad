import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true;
        }

        CText {
            Layout.fillWidth: true

            label: qsTr("Device connected")
            fontSize: 28
            hAlign: Text.AlignHCenter
        }

        CButton {
            Layout.alignment: Qt.AlignHCenter

            label: "Settings"
            toolTipText: "Configure the device"

            onButtonClicked: {
                stack.push(settingsView)
            }
        }

        Item {
            Layout.fillHeight: true;
        }
    }

    Component {
        id: settingsView
        DeviceSettingsView {}
    }
}
