import QtQuick
import QtQuick.Controls.Basic

import Components

Cell {
    id: encoder

    property string actionId: ""
    property string actionDisplayName: ""
    property string actionIconName: ""

    contentItem: Item {
        anchors.fill: parent

        CIcon {
            anchors.centerIn: parent
            iconSize: encoder.width * 0.6

            source: encoder.actionIconName.length ? "qrc:///resources/icons/%1".arg(encoder.actionIconName) : ""
            color: encoder.getTextColor()
            visible: encoder.actionId.length >= 0
        }
    }

    background: Rectangle {
        color: encoder.getBgColor()
        border.color: encoder.getBorderColor()
        border.width: 2
        radius: encoder.width / 2
    }

    CFocusOutline {
        target: encoder
        anchors.fill: parent
        radius: encoder.width / 2
    }

    ToolTip {
        text: encoder.actionDisplayName
        visible: encoder.isHovered && encoder.actionId >= 0 && encoder.actionDisplayName.length > 0
        delay: 600
    }
}
