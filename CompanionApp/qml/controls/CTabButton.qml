import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Button {
    id: button

    property alias buttonRadius: buttonBackground.radius
    property alias iconAnimationType: icon.animationType
    property alias iconToggleAnimation: icon.toggleAnimation
    property alias iconFlipIcon: icon.flipIcon

    signal buttonClicked
    property int buttonHeight: 32
    property int marginSize: 6
    property string iconName
    property string label
    property string toolTipText
    property int fontSize: 14
    property bool expanded: true

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
        anchors.fill: parent
        anchors.margins: 0

        RowLayout {
            anchors.fill: parent
            spacing: 8
            anchors.leftMargin: button.marginSize
            anchors.rightMargin: button.marginSize

            CIcon {
                id: icon

                property int size: button.buttonHeight - 2 * button.marginSize
                Layout.preferredWidth: size
                Layout.preferredHeight: size
                Layout.alignment: Qt.AlignLeft

                source: button.iconName
                iconSize: size
                color: button.getTextColor()
                visible: button.iconName.length > 0
            }

            Text {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                // Text glyphs seem visually lower than I'd like, for now leaving it like this
                Layout.topMargin: -1

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
        id: buttonBackground

        implicitWidth: 80
        implicitHeight: button.buttonHeight
        color: button.getBgColor()
        border.width: 0
        radius: 6
    }

    onPressed: {
        icon.onPressed();
    }

    onReleased: {
        icon.onReleased();
    }

    onCanceled: {
        icon.onReleased();
    }

    onClicked: {
        icon.onClicked();
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: button.toolTipText
        visible: button.hovered && button.enabled && !button.down && !button.expanded && text.length > 0
        delay: 600
    }
}
