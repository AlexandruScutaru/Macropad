import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

TextField {
    id: textField

    signal inputAccepted(value: string)
    property string placeholder
    property string toolTipText
    property int fontSize: 14

    placeholderText: placeholder
    placeholderTextColor: Theme.textSecondary
    color: Theme.textPrimary
    cursorVisible: activeFocus
    font.pointSize: fontSize

    leftPadding: 12
    rightPadding: 12

    horizontalAlignment: TextInput.AlignHCenter

    function isValid() {
        return textField.acceptableInput || text.length === 0;
    }

    background: Rectangle {
        implicitHeight: 32
        color: Theme.backgroundTertiary
        border.color: Theme.error
        border.width: textField.isValid() ? 0 : 1
        radius: height / 2
    }

    onAccepted: {
        if (textField.isValid()) {
            textField.inputAccepted(text);
        }
    }

    ToolTip {
        id: invalidInput
        text: textField.toolTipText
        visible: textField.toolTipText.length > 0 && textField.hovered && !textField.isValid()
        delay: 600
    }
}
