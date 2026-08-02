import QtQuick 2.15
import QtQuick.Layouts

import Components 1.0

CVerticalScrollView {
    id: scrollView

    anchors.fill: parent

    ColumnLayout {
        id: layoutId

        property int scrollBarOffset: 20
        property int maximumWidth: 600

        // trying to limit the size and center the contents, not the cleanest way though
        width: Math.min(scrollView.width - scrollBarOffset, maximumWidth)
        x: (scrollView.width - (width + scrollBarOffset)) / 2

        spacing: 12

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 20

            label: qsTr("Themes")
            fontSize: 16
            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
