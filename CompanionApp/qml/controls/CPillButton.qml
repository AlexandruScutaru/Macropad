import QtQuick
import QtQuick.Controls.Basic

Button {
    id: button

    activeFocusOnTab: true
    focusPolicy: Qt.StrongFocus

    signal buttonClicked
    property string label
    property string toolTipText

    leftPadding: 12
    rightPadding: 12

    text: label

    function getBgColor() {
        if (!button.enabled) return Theme.buttonPrimaryDisabled;
        if (button.down) return Theme.buttonPrimaryPressed;
        if (button.hovered) return Theme.buttonPrimaryHovered;
        return Theme.buttonPrimaryNormal;
    }

    contentItem: Text {
        text: button.text
        font: button.font
        color: enabled ? Theme.textPrimary : Theme.textDisabled
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 32
        color: button.getBgColor()
        border.width: 0
        radius: height / 2
    }

    CFocusOutline {
        target: button
        anchors.fill: button
        radius: height / 2
    }

    onClicked: {
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: button.toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }
}
