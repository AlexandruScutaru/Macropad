import QtQuick
import QtQuick.Controls.Basic

import Controls

Rectangle {
    id: key

    property bool isRound: false
    property bool hovered: false
    property bool selected: false
    property bool dragHover: false

    property string actionId: ""
    property string actionDisplayName: ""
    property string actionIconName: ""

    property string droppedActionId: ""

    signal actionAssigned(actionId: string)
    signal clicked
    signal doubleClicked

    color: getBgColor()
    border.color: getBorderColor()
    border.width: 2
    radius: isRound ? width / 2 : 8

    CIcon {
        anchors.centerIn: parent
        iconSize: key.width * 0.6

        source: key.actionIconName.length ? "qrc:///resources/%1".arg(key.actionIconName) : ""
        color: key.getTextColor()
        visible: key.actionId.length >= 0
    }

    MouseArea {
        anchors.fill: parent

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onEntered: {
            key.hovered = true;
        }

        onExited: {
            key.hovered = false;
            key.dragHover = false;
        }

        onClicked: key.clicked()
        onDoubleClicked: key.doubleClicked()
    }

    DropArea {
        anchors.fill: parent

        keys: ["actionItem"]

        onEntered: (drag) => {
            key.dragHover = true;
        }

        onExited: () => {
            key.dragHover = false;
        }

        onDropped: (drag) => {
            if (key.parseDroppedData(drag.text)) {
                key.dragHover = false;
                key.actionAssigned(key.droppedActionId);
            }
        }
    }

    ToolTip {
        id: tooltip
        text: key.actionDisplayName
        visible: key.hovered && key.actionId >= 0 && key.actionDisplayName.length > 0
        delay: 600
    }

    function parseDroppedData(data: string): bool {
        try {
            const jsonObj = JSON.parse(data);
            if (!jsonObj) throw new Error("couldn't parse json data");
            if (!jsonObj.actionId) throw new Error("couldn't find property 'actionId' in mimeData");

            droppedActionId = jsonObj.actionId

            return true;
        } catch(e) {
            console.error("Couldn't process drag mimeData", e);
        }

        return false;
    }

    function getBgColor() {
        if (hovered || dragHover) return Theme.backgroundTertiary;
        if (selected) return Theme.backgroundSecondary;
        return Theme.backgroundSecondary;
    }

    function getBorderColor() {
        if (hovered || dragHover) return Theme.accentPrimaryHovered;
        if (selected) return Theme.accentPrimaryNormal;
        return Theme.backgroundTertiary;
    }

    function getTextColor() {
        if (key.hovered || key.selected) return Theme.textPrimary;
        return Theme.textSecondary;
    }
}
