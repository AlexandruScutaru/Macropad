import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Button {
    id: button

    signal buttonClicked
    property string label
    property string toolTipText

    leftPadding: 10
    rightPadding: 10

    text: label

    function getBgColor() {
        if (!button.enabled) return "#3e443b";
        if (button.down) return "#4e5946";
        if (button.hovered) return "#485441";
        return "#3e4a36";
    }

    contentItem: Text {
        text: button.text
        font: button.font
        color: enabled ? "#d9e7cb" : "#80ffffff"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 32
        color: getBgColor()
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
