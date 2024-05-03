import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    ColumnLayout {
        anchors.fill: parent

        CText {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            label: qsTr("Device connected")
            fontSize: 28
            hAlign: Text.AlignHCenter
        }

        CButton {
            id: configB
            Layout.alignment: Qt.AlignBottom | Qt.AlignRight

            label: "Configure"
            toolTipText: "Configure the device"

            onButtonClicked: {
                console.log("Configure clicked");
            }
        }
    }
}
