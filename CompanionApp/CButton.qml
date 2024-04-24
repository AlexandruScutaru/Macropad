import QtQuick 2.15
import QtQuick.Controls.Basic 2.15


Button {
    id: button

    signal buttonClicked
    property string label
    property string toolTipText

    text: label

    leftPadding: 10
    rightPadding: 10

    contentItem: Text {
        text: button.text
        font: button.font
        color: "#d9e7cb"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 32
        color: button.down ? "#4e5946"
                           : button.hovered ? "#485441" : "#3e4a36"
        border.width: 0
        radius: height / 2
    }

    onClicked: {
        button.buttonClicked();
    }

    ToolTip {
        id: tooltip
        text: toolTipText
        visible: button.hovered
        delay: 600
    }

}
