import QtQuick
import QtQuick.Layouts

import Controls

Item {
    id: notConnectedView

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            Layout.preferredWidth: 150
            Layout.preferredHeight: 150
            Layout.alignment: Qt.AlignHCenter

            color: "transparent"

            AnimatedImage {
                id: animation
                anchors.fill: parent
                source: "qrc:///resources/anims/not_connected_animation_%1.gif".arg(Theme.isDark() ? "dark" : "light");
                fillMode: Image.PreserveAspectFit
                onStatusChanged: playing = (status == AnimatedImage.Ready)
            }
        }

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 20

            label: qsTr("Cannot connect to the device")
            fontSize: 20
            hAlign: Text.AlignHCenter
        }

        CText {
            Layout.fillWidth: true

            label: qsTr("Make sure the device is plugged into the computer.")
            fontSize: 14
            color: Theme.textSecondary
            hAlign: Text.AlignHCenter
        }

        CButton {
            id: retryButton
            Layout.topMargin: 20
            Layout.alignment: Qt.AlignHCenter

            label: qsTr("Try again")
            toolTipText: qsTr("Retry device connection")

            onButtonClicked: {
                appStackView.deviceConnectTryAgainClicked();
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
