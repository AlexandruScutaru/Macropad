import QtQuick
import QtQuick.Controls.Basic

ScrollView {
    id: scrollView

    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    Binding {
        target: scrollView.contentItem
        property: "boundsBehavior"
        value: Flickable.StopAtBounds
    }

    ScrollBar.vertical: ScrollBar {
        id: vScrollBar

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        orientation: Qt.Vertical
        policy: scrollView.contentHeight > scrollView.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff

        contentItem: Rectangle {
            implicitWidth: (vScrollBar.hovered || vScrollBar.pressed) ? 8 : 4
            radius: width / 2
            color: (vScrollBar.hovered || vScrollBar.pressed) ? Theme.buttonSecondaryHovered : Theme.buttonSecondaryPressed
        }
    }
}
