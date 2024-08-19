import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.alignment: Qt.AlignVCenter

            label: qsTr("Connect to a device")
            fontSize: 20
            hAlign: Text.AlignLeft
        }

        DeviceSearchHeader {
            Layout.fillWidth: true
            Layout.topMargin: 16

            onSearchClicked: (vid, pid) => {
                mainController.search(vid, pid);
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 8

            focus: false

            color: "#2d2d2d"
            radius: 8
            z: -1

            ListView {
                id: listView

                anchors.fill: parent
                anchors.margins: 4
                anchors.rightMargin: 0
                clip: true
                boundsMovement: Flickable.StopAtBounds
                focus: true
                spacing: 4

                model: mainController.getDeviceInfoModel()
                delegate: DeviceListDelegate {}

                function getSelectedDevicePath() {
                    return currentItem.path;
                }

                onCurrentIndexChanged: {
                    connectButton.enabled = true;
                }

                ScrollBar.vertical: ScrollBar {
                    id: vScrollBar
                    active: true
                    policy: listView.contentHeight > listView.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    orientation: Qt.Vertical

                    contentItem: Rectangle {
                        implicitWidth: (vScrollBar.hovered || vScrollBar.pressed) ? 8 : 4
                        radius: width / 2
                        color: (vScrollBar.hovered || vScrollBar.pressed) ? "#70ffffff" : "#50ffffff"
                    }
                }
            }
        }

        CButton {
            id: connectButton
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 16

            label: "Connect"
            toolTipText: "Connect to the selected device"
            enabled: false

            onButtonClicked: {
                mainController.openDevice(listView.getSelectedDevicePath());
            }
        }
    }
}
