import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: icon

    property string source
    property color color: Theme.textPrimary
    property int iconSize: 40
    property int iconWidth: iconSize
    property int iconHeight: iconSize

    width: iconWidth
    height: iconHeight

    Image {
        id: iconImage
        anchors.fill: parent
        source: icon.source
    }

    ColorOverlay {
        anchors.fill: iconImage
        source: iconImage
        color: icon.color
    }
}
