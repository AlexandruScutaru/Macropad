import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

TextField {
    id: textField

    signal inputAccepted(value: string)
    property string placeholder

    placeholderText: placeholder
    placeholderTextColor: "#80ffffff"
    cursorVisible: activeFocus
    color: "#d9e7cb"
    font.pointSize: 12

    validator: RegularExpressionValidator { regularExpression: /[0-9A-Fa-f]{0}([0-9A-Fa-f]{4})?/ }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: "transparent"
        border.color: "red"
        border.width: acceptableInput ? 0 : 1
    }

    onAccepted: {
        if (acceptableInput || text.length === 0) {
            textField.inputAccepted(text);
        }
    }

    ToolTip {
        id: invalidInput
        text: "Input is not valid! Either leave the field empty or type a 4-digit hex number"
        visible: !acceptableInput && textField.hovered
        delay: 600
    }
}
