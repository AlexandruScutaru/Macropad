import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 12

            label: qsTr("Change output - F13")
            fontSize: 14
            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
