import QtQuick
import QtQuick.Controls.Basic

import Controls

Control {
    id: key

    activeFocusOnTab: false
    focus: false

    // disabling it so tab can't reach this normally,
    // only when user explicitly enters the grid via enter/return press
    // focusPolicy: Qt.StrongFocus

    property bool isRound: false
    property bool selected: false
    property bool dragHover: false

    property string actionId: ""
    property string actionDisplayName: ""
    property string actionIconName: ""

    property string droppedActionId: ""

    signal actionAssigned(actionId: string)
    signal clicked
    signal doubleClicked

    implicitWidth: 64
    implicitHeight: 64

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Space) {
            key.clicked();
            event.accepted = true;
        } else if (selected && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
            key.doubleClicked();
            event.accepted = true;
        }
    }

    contentItem: Item {
        anchors.fill: parent

        CIcon {
            anchors.centerIn: parent
            iconSize: key.width * 0.6

            source: key.actionIconName.length ? "qrc:///resources/icons/%1".arg(key.actionIconName) : ""
            color: key.getTextColor()
            visible: key.actionId.length >= 0
        }
    }

    background: Rectangle {
        id: backgroundRect
        color: key.getBgColor()
        border.color: key.getBorderColor()
        border.width: 2
        radius: key.isRound ? key.width / 2 : 8
    }

    CFocusOutline {
        target: key
        anchors.fill: parent
        radius: key.isRound ? key.width / 2 : 8
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onExited: {
            key.dragHover = false;
        }

        onClicked: {
            key.forceActiveFocus();
            key.clicked();
        }

        onDoubleClicked: {
            key.doubleClicked();
        }
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
        visible: mouseArea.containsMouse && key.actionId >= 0 && key.actionDisplayName.length > 0
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
        if (mouseArea.pressed) return Theme.buttonSecondaryPressed;
        if (mouseArea.containsMouse || dragHover) return Theme.buttonSecondaryHovered;
        return Theme.backgroundTertiary;
    }

    function getBorderColor() {
        if (dragHover) return Theme.accentPrimaryHovered;
        if (selected) return Theme.accentPrimaryNormal;
        return Theme.backgroundTertiary;
    }

    function getTextColor() {
        if (mouseArea.containsMouse || key.selected) return Theme.textPrimary;
        return Theme.textSecondary;
    }
}
