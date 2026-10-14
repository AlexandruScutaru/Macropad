import QtQuick
import QtQuick.Controls.Basic

Control {
    id: cell

    activeFocusOnTab: false
    focus: false

    // disabling it so tab can't reach this normally,
    // only when user explicitly enters the grid via enter/return press
    // focusPolicy: Qt.StrongFocus

    required property int cellIndex

    property bool selected: false
    property bool dragHover: false
    property string droppedActionId: ""
    property int navigationColumns: 1
    property var navigationItems: null

    property alias isHovered: mouseArea.containsMouse

    signal actionAssigned(actionId: string)
    signal clicked
    signal doubleClicked

    implicitWidth: 64
    implicitHeight: 64

    KeyNavigation.left: !navigationItems || cellIndex % navigationColumns === 0 ? null : navigationItems.itemAt(cellIndex - 1)
    KeyNavigation.right: !navigationItems || cellIndex % navigationColumns === navigationColumns - 1 ? null : navigationItems.itemAt(cellIndex + 1)
    KeyNavigation.up: !navigationItems || cellIndex < navigationColumns ? null : navigationItems.itemAt(cellIndex - navigationColumns)
    KeyNavigation.down: !navigationItems || cellIndex + navigationColumns >= navigationItems.count ? null : navigationItems.itemAt(cellIndex + navigationColumns)

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Space) {
            cell.clicked();
            event.accepted = true;
        } else if (selected && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
            cell.doubleClicked();
            event.accepted = true;
        }
    }

    contentItem: Item {
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onExited: {
            cell.dragHover = false;
        }

        onClicked: {
            cell.forceActiveFocus();
            cell.clicked();
        }

        onDoubleClicked: {
            cell.doubleClicked();
        }
    }

    DropArea {
        anchors.fill: parent

        keys: ["actionItem"]

        onEntered: {
            cell.dragHover = true;
        }

        onExited: {
            cell.dragHover = false;
        }

        onDropped: (drag) => {
            if (cell.parseDroppedData(drag.text)) {
                cell.dragHover = false;
                cell.actionAssigned(cell.droppedActionId);
            }
        }
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
        if (mouseArea.containsMouse || selected) return Theme.textPrimary;
        return Theme.textSecondary;
    }
}
