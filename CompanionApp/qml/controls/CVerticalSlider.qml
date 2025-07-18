import QtQuick
import QtQuick.Controls.Basic

Slider {
    id: slider

    signal sliderMoved(value: int)
    property int start: 0
    property int end: 100
    property int current

    orientation: Qt.Vertical

    width: 24
    padding: 0

    from: start
    value: current
    to: end

    background: Rectangle {
        x: slider.leftPadding + slider.width / 2 - width / 2
        y: slider.topPadding
        implicitWidth: 2
        implicitHeight: slider.height
        width: implicitWidth
        height: slider.height
        color: Theme.accentPrimaryNormal

        Rectangle {
            width: 1
            height: slider.visualPosition * parent.height
            radius: 1
            color: Theme.textPrimary
        }
    }

    handle: Rectangle {
        x: slider.leftPadding + slider.width / 2 - width / 2
        y: slider.topPadding + slider.visualPosition * (slider.height - height)
        implicitWidth: 24
        implicitHeight: 24
        radius: 12
        color: slider.hovered ? Theme.accentPrimaryHovered : Theme.accentPrimaryNormal
    }

    onMoved: {
        slider.sliderMoved(slider.value);
    }
}
