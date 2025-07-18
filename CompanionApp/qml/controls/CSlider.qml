import QtQuick
import QtQuick.Controls.Basic

Slider {
    id: slider

    signal sliderMoved(value: int)
    property int start: 0
    property int end: 100
    property int current

    height: 24
    padding: 0

    from: start
    value: current
    to: end

    background: Rectangle {
        x: slider.leftPadding
        y: slider.topPadding + slider.height / 2 - height / 2
        implicitWidth: slider.width
        implicitHeight: 1
        width: slider.width
        height: implicitHeight
        color: Theme.textPrimary

        Rectangle {
            width: slider.visualPosition * parent.width
            height: 2
            radius: 1
            color: Theme.accentPrimaryNormal
        }
    }

    handle: Rectangle {
        x: slider.leftPadding + slider.visualPosition * (slider.width - width)
        y: slider.topPadding + slider.height / 2 - height / 2
        implicitWidth: 24
        implicitHeight: 24
        radius: 12
        color: slider.hovered ? Theme.accentPrimaryHovered : Theme.accentPrimaryNormal
    }

    onMoved: {
        slider.sliderMoved(slider.value);
    }
}
