import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Window {
    id: mainWindow

    width: 256
    height: 128
    minimumWidth: 256
    minimumHeight: 128
    visible: true
    title: qsTr("Macropad Companion")
    color: "#222222"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        Foo {
            Layout.fillWidth: true
        }

        Bar {
            Layout.fillWidth: true
        }
    }
}
