import QtQuick
import QtQuick.Controls.Basic

import Components

Cell {
    id: button

    property string actionId: ""
    property string actionDisplayName: ""
    property string actionIconName: ""

    contentItem: Item {
        anchors.fill: parent

        CIcon {
            anchors.centerIn: parent
            iconSize: button.width * 0.6

            source: button.actionIconName.length ? "qrc:///resources/icons/%1".arg(button.actionIconName) : ""
            color: button.getTextColor()
            visible: button.actionId.length >= 0
        }
    }

    background: Rectangle {
        color: button.getBgColor()
        border.color: button.getBorderColor()
        border.width: 2
        radius: 8
    }

    CFocusOutline {
        target: button
        anchors.fill: parent
        radius: 8
    }

    ToolTip {
        text: button.actionDisplayName
        visible: button.isHovered && button.actionId >= 0 && button.actionDisplayName.length > 0
        delay: 600
    }
}
