import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Slider {
    id: slider

    signal sliderMoved(value: int)
    property int start: 0
    property int end: 100
    property int current

    orientation: Qt.Vertical

    from: start
    value: current
    to: end

    background: Rectangle {
        x: slider.leftPadding + slider.availableWidth / 2 - width / 2
        y: slider.topPadding
        implicitWidth: 2
        implicitHeight: 200
        width: implicitWidth
        height: slider.availableHeight
        color: "#485441"

        Rectangle {
            width: 1
            height: slider.visualPosition * parent.height
            radius: 1
            color: "#d9e7cb"
        }
    }

    handle: Rectangle {
        x: slider.leftPadding + slider.availableWidth / 2 - width / 2
        y: slider.topPadding + slider.visualPosition * (slider.availableHeight - height)
        implicitWidth: 24
        implicitHeight: 24
        radius: 12
        color: slider.hovered ? "#485441" : "#3e4a36"
    }

    onMoved: {
        slider.sliderMoved(slider.value);
    }
}
