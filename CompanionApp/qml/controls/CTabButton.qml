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
        if (!button.enabled) return "#2d2d2d";
        if (button.down) return "#383838";
        if (button.hovered) return "#484848";
        if (button.checked) return "#3d3d3d";
        return "transparent";
    }

    contentItem: Text {
        text: button.text
        font: button.font
        color: enabled ? "#d9e7cb" : "#80ffffff"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 32
        color: getBgColor()
        border.width: 0
        radius: 6
    }

    onClicked: {
        toggled
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }
}
