import QtQuick 2.15
import QtQuick.Layouts

import Controls 1.0

Rectangle {
    id: search

    signal searchClicked(vid: string, pid: string)

    height: 40
    Layout.fillWidth: true
    radius: height / 2

    color: "#2d2d2d"

    function onSearchRequested() {
        if (vid.acceptableInput && pid.acceptableInput) {
            searchClicked(vid.text, pid.text);
        }
    }

    RowLayout {
        id: mainLayout

        anchors.fill: parent
        anchors {
            leftMargin: 8
            rightMargin: 8
            topMargin: 4
            bottomMargin: 4
        }

        spacing: 0

        CTextField {
            id: vid
            Layout.fillHeight: true
            Layout.fillWidth: true

            placeholder: qsTr("Vendor ID")

            onInputAccepted: (value) => {
                onSearchRequested();
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            color: "#70ffffff"
            width: 1
        }

        CTextField {
            id: pid
            Layout.fillHeight: true
            Layout.fillWidth: true

            placeholder: qsTr("Product ID")

            onInputAccepted: (value) => {
                onSearchRequested();
            }
        }

        Item {
            Layout.fillWidth: true
        }

        CIconButton {
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: 8

            iconName: "qrc:///resources/search_icon.png"
            toolTipText: "Search for this device"
            iconPadding: 8

            onButtonClicked: {
                onSearchRequested();
            }
        }
    }
}
