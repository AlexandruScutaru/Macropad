import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0


Item {
    id: notConnectedView

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            width: 150
            height: 150
            Layout.alignment: Qt.AlignHCenter

            color: "transparent"

            AnimatedImage {
                id: animation
                anchors.fill: parent
                source: "qrc:///resources/not_connected_animation.gif"
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
            color: "#80ffffff"
            hAlign: Text.AlignHCenter
        }

        CButton {
            id: retryButton
            Layout.topMargin: 20
            Layout.alignment: Qt.AlignHCenter

            label: qsTr("Try again")
            toolTipText: qsTr("Retry device connection")

            onButtonClicked: {
                root.tryAgainClicked();
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
