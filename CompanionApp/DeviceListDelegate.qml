import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Rectangle {
    id: deviceEntry

    required property string vid
    required property string pid
    required property string usagePage
    required property string usageId
    required property string product
    required property string manufacturer
    required property string serial
    required property string path
    required property int index

    width: list.width - 12
    height: 30
    radius: 8

    color: ListView.isCurrentItem ? "#3d3d3d" : mouseArea.hovered ? "#4d4d4d" : "#2d2d2d"

    RowLayout {
        id: mainLayout

        anchors.fill: parent
        anchors {
            leftMargin: 4
            rightMargin: 4
            topMargin: 0
            bottomMargin: 0
        }

        spacing: 0

        CText {
            Layout.fillHeight: true
            Layout.fillWidth: true

            label: manufacturer + " " + product
            fontSize: 12
            hAlign: Text.AlignLeft
        }

        CIconButton {
            id: infoButton

            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 4

            iconName: "qrc:///resources/info_icon.png"
            toolTipText: "See details"
            size: 28

            onButtonClicked: {
                popup.visible = true;
            }
        }
    }

    MouseArea {
        id: mouseArea

        property bool hovered

        hoverEnabled: true
        anchors.fill: parent
        anchors.rightMargin: 32

        onEntered: {
            hovered = true
        }

        onExited: {
            hovered = false
        }

        onClicked: {
            list.currentIndex = index
        }
    }

    Popup {
        id: popup
        x: 0
        y: 0
        width: 300
        height: 200
        modal: false
        focus: true
        dim: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        visible: false

        ColumnLayout {
            anchors.fill: parent
            spacing: 4

            CText {
                Layout.fillWidth: true
                label: vid
            }
            CText {
                Layout.fillWidth: true
                label: pid
            }
            CText {
                Layout.fillWidth: true
                label: usagePage
            }
            CText {
                Layout.fillWidth: true
                label: usageId
            }
            CText {
                Layout.fillWidth: true
                label: serial
            }
            CText {
                Layout.fillWidth: true
                label: path
            }
        }
    }
}
