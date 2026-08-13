import QtQuick
import QtQuick.Controls.Basic

import Components

RadioButton {
    id: themeSelector

    property alias model: preview.model

    activeFocusOnTab: true
    focusPolicy: Qt.StrongFocus

    signal themeSelected
    property string label
    property string toolTipText
    property bool selected

    checked: selected

    implicitWidth: selectionIndicator.implicitWidth
    implicitHeight: selectionIndicator.implicitHeight

    onCheckedChanged: {
        if (checked) {
            themeSelector.themeSelected();
        }
    }

    indicator: Rectangle {
        id: selectionIndicator

        implicitWidth: preview.implicitWidth + 12
        implicitHeight: preview.implicitHeight + 12

        color: "transparent"
        border.color: themeSelector.getBorderColor()
        border.width: 2

        radius: 8

        ThemePreview {
            id: preview

            anchors.centerIn: parent
        }
    }

    contentItem: Item {}

    function getBorderColor() {
        if (themeSelector.down) return Theme.accentPrimaryNormal;
        if (themeSelector.hovered) return Theme.accentPrimaryHovered;
        if (themeSelector.checked) return Theme.accentPrimaryNormal;
        return "transparent";
    }

    CFocusOutline {
        target: themeSelector
        anchors.fill: selectionIndicator
        radius: selectionIndicator.radius
    }

    ToolTip {
        id: tooltip
        text: themeSelector.toolTipText
        visible: themeSelector.hovered && themeSelector.enabled && !themeSelector.down && text.length > 0
        delay: 600

        x: selectionIndicator.x
    }
}
