import QtQuick
import QtQuick.Controls.Basic

Button {
    id: button

    signal keyBindRecorded(key: int)
    property int key
    property string label: key === Qt.Key_unknown ? "" : "F%1".arg(13 + key - Qt.Key_F13);
    property string toolTipText

    QtObject {
        id: props
        property bool recording: false
    }

    focus: false
    leftPadding: 0
    rightPadding: 0

    font.pointSize: 14
    font.weight: 600
    text: label

    function getBgColor() {
        if (!button.enabled) return "transparent";
        if (button.down) return "#3d3d3d";
        if (button.hovered) return "#484848";
        if (button.checked) return "#3d3d3d";
        return "#383838";
    }

    contentItem: Text {
        text: button.text
        font: button.font
        color: enabled ? "#d9e7cb" : "#80ffffff"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        property int buttonSize: 40

        implicitWidth: buttonSize
        implicitHeight: buttonSize
        width: buttonSize
        height: buttonSize
        color: getBgColor()
        border.color: "#d9e7cb"
        border.width: props.recording ? 1 : 0
        radius: 6
    }

    onClicked: {
        props.recording = !props.recording
    }

    Keys.onPressed: (event) => {
        if (!props.recording) {
            return;
        }

        event.accepted = true;
        props.recording = false;
        button.focus = false;

        if (event.key === Qt.Key_Delete) {
            button.key = Qt.Key_unknown
            label = "";
            keyBindRecorded(button.key)
            return;
        }

        if (event.key === Qt.Key_Escape || event.count > 1 || event.modifiers !== Qt.NoModifier ||
            event.key < Qt.Key_F13 || event.key > Qt.Key_F24)
        {
            return;
        }

        button.key = event.key
        label = "F%1".arg(13 + button.key - Qt.Key_F13);
        keyBindRecorded(button.key);
    }

    ToolTip {
        id: tooltip
        text: toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }

}
