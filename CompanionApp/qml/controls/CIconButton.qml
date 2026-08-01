import QtQuick
import QtQuick.Controls.Basic

Button {
    id: button

    property alias iconAnimationType: icon.animationType
    property alias iconToggleAnimation: icon.toggleAnimation
    property alias iconFlipIcon: icon.flipIcon

    signal buttonClicked
    property string iconName
    property string toolTipText
    property int buttonSize: 32
    property int iconPadding: 6

    CIcon {
        id: icon

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

    function getBgColor() {
        if (!button.enabled) return Theme.buttonSecondaryDisabled;
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    ToolTip {
        id: tooltip
        text: button.toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }
}
