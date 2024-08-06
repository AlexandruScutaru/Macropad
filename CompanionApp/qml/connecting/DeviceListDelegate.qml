import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

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

    width: listView.width - 12
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
            buttonSize: 28
            iconPadding: 6

            onButtonClicked: {
                popup.openAt(mapToItem(parent, deviceEntry.x, deviceEntry.y));
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
            listView.currentIndex = index
        }
    }

    Popup {
        id: popup

        width: deviceEntry.width
        height: 200
        margins: 0
        padding: 4

        modal: true
        focus: true
        dim: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        visible: false
        background: Rectangle {
            color: "#2d2d2d"
            border.width: 1
            border.color: "#4d4d4d"
            radius: 8
        }

        parent: listView

        function openAt(coords: point) {
            popup.x = 0;
            popup.y = coords.y - listView.visibleArea.yPosition * listView.contentHeight;
            popup.open();
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0

            RowLayout {
                spacing: 8

                CText {
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true

                    label: manufacturer + " " + product
                    fontSize: 12
                }

                CIconButton {
                    id: closeButton

                    Layout.alignment: Qt.AlignRight | Qt.AlignTop

                    iconName: "qrc:///resources/close_icon.png"
                    toolTipText: ""
                    buttonSize: 28
                    iconPadding: 6

                    onButtonClicked: {
                        popup.close();
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            InfoRow {
                rowLabel: "Vendor ID:"
                rowValue: vid
            }

            InfoRow {
                rowLabel: "Product ID:"
                rowValue: pid
            }

            InfoRow {
                rowLabel: "Usage Page:"
                rowValue: usagePage
            }

            InfoRow {
                rowLabel: "Usage ID:"
                rowValue: usageId
            }

            InfoRow {
                rowLabel: "Serial ID:"
                rowValue: serial
            }

            InfoRow {
                rowLabel: "Path:"
                rowValue: path
                valueWrap: Text.WrapAnywhere
                showSeparator: false
            }

            Item {
                Layout.fillHeight: true
            }
        }

        component InfoRow : ColumnLayout {
            property alias rowLabel: label.label
            property alias rowValue: value.label
            property alias valueWrap: value.wrap
            property alias showSeparator: separatorLine.visible

            spacing: 0

            RowLayout {
                spacing: 8
    
                CText {
                    id: label
                    Layout.preferredWidth: 100
                    fontSize: 10
                }
    
                CText {
                    id: value
                    Layout.fillWidth: true
                    fontSize: 10
                }
            }

            Rectangle {
                id: separatorLine

                Layout.fillWidth: true
                Layout.topMargin: 2
                height: 1
                color: "#242424"
            }
        }
    }
}
