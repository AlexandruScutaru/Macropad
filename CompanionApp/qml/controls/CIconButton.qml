import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import Qt5Compat.GraphicalEffects

Button {
    id: button

    signal buttonClicked
    property string iconName
    property string toolTipText
    property int buttonSize: 32
    property int iconPadding: 4

    Image {
        id: iconImage
        anchors.centerIn: parent
        source: iconName
        width: buttonSize - iconPadding * 2
        height: buttonSize - iconPadding * 2
        mipmap: true
    }

    ColorOverlay {
        anchors.fill: iconImage
        source: iconImage
        color: "#d9e7cb"
    }

    background: Rectangle {
        implicitWidth: buttonSize
        implicitHeight: buttonSize
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
