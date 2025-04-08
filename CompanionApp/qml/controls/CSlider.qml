import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Slider {
    id: slider

    signal sliderMoved(value: int)
    property int start: 0
    property int end: 100
    property int current

    from: start
    value: current
    to: end

    background: Rectangle {
        x: slider.leftPadding
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 1
        width: slider.availableWidth
        height: implicitHeight
        color: "#d9e7cb"

        Rectangle {
            width: slider.visualPosition * parent.width
            height: parent.height
            implicitHeight: 2
            radius: 1
            color: "#485441"
        }
    }

    handle: Rectangle {
        x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
        y: slider.topPadding + slider.availableHeight / 2 - height / 2
        implicitWidth: 24
        implicitHeight: 24
        radius: 12
        color: slider.hovered ? "#485441" : "#3e4a36"
    }

    onMoved: {
        slider.sliderMoved(slider.value);
    }
}
