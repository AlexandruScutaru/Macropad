import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

Button {
    id: button

    readonly property int horizontalMargin: 12
    readonly property int buttonHeight: 40

    signal buttonClicked
    property string iconName
    property string label
    property string toolTipText
    property int fontSize: 14
    property bool expanded: false

    font.pointSize: fontSize
    text: label
    anchors.margins: 0
    padding: 0

    function getBgColor() {
        if (!button.enabled) return Theme.buttonSecondaryDisabled;
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.checked) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    function getTextColor() {
        if (!button.enabled) return Theme.textDisabled;
        if (button.hovered) return Theme.textPrimary;
        if (!button.checked) return Theme.textSecondary;
        return Theme.textPrimary;
    }

    contentItem: Item {
        anchors.margins: 0

        RowLayout {
            spacing: 12
            anchors.fill: parent
            anchors.leftMargin: button.horizontalMargin - 2
            anchors.rightMargin: button.horizontalMargin - 2

            CIcon {
                property int size: button.buttonHeight - 2 * button.horizontalMargin
                Layout.preferredWidth: size
                Layout.preferredHeight: size

                source: button.iconName
                iconSize: size
                color: button.getTextColor()
                visible: button.iconName.length > 0
            }

            Text {
                Layout.fillWidth: true

                text: button.text
                font: button.font
                color: button.getTextColor()
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                visible: button.expanded
            }
        }
    }

    background: Rectangle {
        implicitWidth: 80
        implicitHeight: button.buttonHeight
        color: button.getBgColor()
        border.width: 0
        radius: 6
    }

    onClicked: {
        toggled
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: button.toolTipText
        visible: button.hovered && button.enabled && !button.down && !button.expanded && text.length > 0
        delay: 600
    }
}
