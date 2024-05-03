import QtQuick 2.15
import QtQuick.Controls.Basic 2.15


Button {
    id: button

    signal buttonClicked
    property string iconName
    property string toolTipText
    property int size: 32

    icon.color: "#d9e7cb"
    icon.source: iconName
    icon.width: size / 2
    icon.height: size / 2

    background: Rectangle {
        implicitWidth: size
        implicitHeight: size
        color: button.down ? "#484848"
                           : button.hovered ? "#4d4d4d" : "transparent"
        border.width: 0
        radius: height / 2
    }

    onClicked: {
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }

}
