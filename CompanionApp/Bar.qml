import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    height: content.height

    ColumnLayout {
        id: content
        width: parent.width

        Button {
            id: outputButton
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Change Output")

            leftPadding: 10
            rightPadding: 10

            contentItem: Text {
                text: outputButton.text
                font: outputButton.font
                color: "#d9e7cb"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 48
                implicitHeight: 32
                color: outputButton.down ? "#4e5946" : outputButton.hovered ? "#485441" : "#3e4a36"
                border.width: 0
                radius: width / 2
            }

            onClicked: {}
        }
    }
}
