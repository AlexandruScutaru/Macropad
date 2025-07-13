import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Button {
    id: button

    signal buttonClicked
    property string label
    property string toolTipText
    property int fontSize: 12

    leftPadding: 10
    rightPadding: 10

    font.pointSize: fontSize
    text: label

    function getBgColor() {
        if (!button.enabled) return Theme.buttonSecondaryDisabled;
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.checked) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    contentItem: Text {
        text: button.text
        font: button.font
        color: enabled ? Theme.textPrimary : Theme.textDisabled
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 32
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
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }
}
