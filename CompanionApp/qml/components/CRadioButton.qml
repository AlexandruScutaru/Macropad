import QtQuick
import QtQuick.Controls.Basic


RadioButton {
    id: radio

    activeFocusOnTab: true
    focusPolicy: Qt.StrongFocus

    signal optionSelected
    property string label
    property string toolTipText
    property bool selected

    text: label
    checked: selected

    onCheckedChanged: {
        if (checked) {
            radio.optionSelected();
        }
    }

    indicator: Rectangle {
        id: radioIndicator

        implicitWidth: 26
        implicitHeight: 26
        x: radio.leftPadding
        y: parent.height / 2 - height / 2

        radius: height / 2

        color: "transparent"
        border.color: radio.getBgColor()

        Rectangle {
            width: parent.width / 2
            height: parent.height / 2
            x: parent.width / 2 - width / 2
            y: parent.height / 2 - height / 2

            radius: height / 2

            color: radio.getBgColor()
            visible: radio.checked
        }
    }

    contentItem: Text {
        text: radio.text
        font: radio.font
        color: radio.getTextColor()
        verticalAlignment: Text.AlignVCenter
        leftPadding: radio.indicator.width + radio.spacing
    }

    function getBgColor() {
        if (!radio.enabled) return Theme.buttonPrimaryDisabled;
        if (radio.down) return Theme.buttonPrimaryPressed;
        if (radio.hovered) return Theme.buttonPrimaryHovered;
        if (radio.checked) return Theme.buttonPrimaryPressed;
        return Theme.buttonPrimaryNormal;
    }

    function getTextColor() {
        if (!radio.enabled) return Theme.textDisabled;
        if (radio.hovered) return Theme.textPrimary;
        if (!radio.checked) return Theme.textSecondary;
        return Theme.textPrimary;
    }

    CFocusOutline {
        target: radio
        anchors.fill: radioIndicator
        radius: radioIndicator.radius
    }

    ToolTip {
        id: tooltip
        text: radio.toolTipText
        visible: radio.hovered && radio.enabled && !radio.down && text.length > 0
        delay: 600

        x: radioIndicator.x
    }
}
