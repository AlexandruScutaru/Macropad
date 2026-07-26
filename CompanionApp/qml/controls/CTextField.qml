import QtQuick
import QtQuick.Controls.Basic

TextField {
    id: textField

    signal inputAccepted(value: string)
    property string placeholder
    property string toolTipText
    property int fontSize: 12
    property int radius: height / 2

    placeholderText: placeholder
    placeholderTextColor: Theme.textSecondary
    color: Theme.textPrimary
    cursorVisible: activeFocus
    font.pointSize: fontSize

    leftPadding: 12
    rightPadding: 12

    horizontalAlignment: TextInput.AlignLeft

    function isValid() {
        return textField.acceptableInput || text.length !== 0;
    }

    function handleText() {
        if (textField.isValid()) {
            textField.inputAccepted(text);
        }
    }

    background: Rectangle {
        implicitHeight: 20
        color: Theme.backgroundTertiary
        border.color: Theme.error
        border.width: textField.isValid() ? 0 : 1
        radius: textField.radius
    }

    onAccepted: textField.handleText()
    onEditingFinished: textField.handleText()

    // onActiveFocusChanged: {
    //     if (!activeFocus) {
    //         textField.handleText()
    //     }
    // }

    ToolTip {
        id: invalidInput
        text: textField.toolTipText
        visible: textField.toolTipText.length > 0 && textField.hovered && !textField.isValid()
        delay: 600
    }
}
