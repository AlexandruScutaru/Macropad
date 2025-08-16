import QtQuick
import QtQuick.Layouts

import Controls

Item {
    id: keyconfig

    property alias configModel: optionsRepeater.model

    CVerticalScrollView {
        id: scrollView

        anchors.fill: parent
        anchors.margins: 0

        ColumnLayout {
            width: scrollView.width
            spacing: 0

            Repeater {
                id: optionsRepeater
            }
        }
    }
}
