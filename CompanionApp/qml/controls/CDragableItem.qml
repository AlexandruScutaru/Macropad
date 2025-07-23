import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Rectangle {
    id: item

    property int itemHeight: 40
    property int marginSize: 12
    property int leftMargin: 12
    property int iconSize: 20
    property string iconName
    property string label
    property string toolTipText
    property int fontSize: 14
    property string mimePayload

    property bool hovered: false
    property bool down: false

    anchors.margins: 0
    implicitWidth: 80
    implicitHeight: itemHeight
    color: getBgColor()
    border.width: 0
    radius: 0

    Drag.mimeData: {
        "actionItem": "",
        "text/plain": item.mimePayload
    }
    Drag.hotSpot.x: 0
    Drag.hotSpot.y: 0
    Drag.dragType: Drag.Automatic

    DragHandler {
        id: dragHandler
        target: null

        onActiveChanged: {
            if (active) {
                draggedItem.grabToImage(function(result) {
                    item.Drag.imageSource = result.url
                    item.Drag.active = true
                }, Qt.size(draggedItem.width, draggedItem.height));
            } else {
                item.Drag.active = false
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: item.marginSize
        anchors.leftMargin: item.leftMargin
        anchors.rightMargin: item.marginSize

        CIcon {
            id: icon

            Layout.preferredWidth: item.iconSize
            Layout.preferredHeight: item.iconSize
            Layout.alignment: Qt.AlignLeft

            source: item.iconName
            iconSize: item.iconSize
            color: item.getTextColor()
            visible: item.iconName.length > 0
        }

        CText {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            Layout.topMargin: -1

            label: item.label
            fontSize: item.fontSize
            color: item.getTextColor()
            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
            elide: Text.ElideRight
        }

        CIcon {
            id: chevronIcon

            property bool expanded: false
            Layout.preferredWidth:  16
            Layout.preferredHeight: 16
            Layout.alignment: Qt.AlignRight

            source: "qrc:///resources/drag_icon.svg"
            iconSize: 16
            color: Theme.textDisabled
        }
    }

    function getBgColor() {
        if (item.down) return Theme.buttonSecondaryPressed;
        if (item.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    function getTextColor() {
        if (item.hovered || item.down) return Theme.textPrimary;
        return Theme.textSecondary;
    }

    MouseArea {
        anchors.fill: parent

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onEntered: { item.hovered = true; }
        onExited: { item.hovered = false; }
        onPressed: { item.down = true; }
        onReleased: { item.down = false; }
        onCanceled: {
            item.down = false;
            item.hovered = false;
        }
    }

    ToolTip {
        id: tooltip
        text: item.toolTipText
        visible: item.hovered && text.length > 0
        delay: 600
    }

    Rectangle {
        id: draggedItem

        width: 140
        height: 40
        radius: 8
        color: Theme.backgroundBackdrop
        anchors.margins: 0
        visible: false

        RowLayout {
            anchors.fill: parent
            spacing: 8
            anchors.margins: 8

            CIcon {
                Layout.preferredWidth: 16
                Layout.preferredHeight: 16
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 12
                // TODO: fix this centering at a later time, focused on other stuff for now
                Layout.topMargin: -1

                source: item.iconName
                iconSize: 16
                color: Theme.textPrimary
            }

            CText {
                Layout.fillWidth: true
                Layout.topMargin: -1

                label: item.label
                fontSize: 12
                color: Theme.textPrimary
                hAlign: Text.AlignLeft
                elide: Text.ElideRight
            }
        }
    }
}
