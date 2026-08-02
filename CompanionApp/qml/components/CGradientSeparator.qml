import QtQuick

Rectangle {
    id: separator

    property int size: 1
    property color colorEnds: "transparent"
    property color colorMiddle: Theme.textPrimary
    property int orientation: Gradient.Horizontal

    width: orientation === Gradient.Horizontal ? width : 1
    height: orientation === Gradient.Horizontal ? 1 : height

    gradient: Gradient {
        orientation: separator.orientation

        GradientStop { position: 0.0; color: separator.colorEnds }
        GradientStop { position: 0.2; color: separator.colorMiddle }
        GradientStop { position: 0.8; color: separator.colorMiddle }
        GradientStop { position: 1.0; color: separator.colorEnds }
    }
}
