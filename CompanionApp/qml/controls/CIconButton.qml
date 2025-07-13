import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import Qt5Compat.GraphicalEffects

Button {
    id: button

    signal buttonClicked
    property string iconName
    property string toolTipText
    property int buttonSize: 32
    property int iconPadding: 5

    function getBgColor() {
        if (!button.enabled) return Theme.buttonSecondaryDisabled;
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    Image {
        id: iconImage
        anchors.centerIn: parent
        source: button.iconName
        width: button.buttonSize - button.iconPadding * 2
        height: button.buttonSize - button.iconPadding * 2
        mipmap: true
    }

    ColorOverlay {
        anchors.fill: iconImage
        source: iconImage
        color: Theme.textPrimary
    }

    background: Rectangle {
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
