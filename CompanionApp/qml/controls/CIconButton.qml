import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Button {
    id: button

    signal buttonClicked
    property string iconName
    property string toolTipText
    property int buttonSize: 40
    property int iconPadding: 12

    function getBgColor() {
        if (!button.enabled) return Theme.buttonSecondaryDisabled;
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    CIcon {
        anchors.centerIn: parent
        source: button.iconName
        iconSize: button.buttonSize - button.iconPadding * 2
        color: Theme.textPrimary
    }

    background: Rectangle {
        width: button.buttonSize
        height: button.buttonSize
        implicitWidth: button.buttonSize
        implicitHeight: button.buttonSize
        color: button.getBgColor()
        border.width: 0
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
