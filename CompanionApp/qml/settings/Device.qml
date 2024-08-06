import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 16

            label: qsTr("Autoconnect last device v")
            fontSize: 14
            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
