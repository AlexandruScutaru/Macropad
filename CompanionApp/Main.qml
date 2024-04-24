import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Window {
    id: mainWindow

    width: 640
    height: 480
    minimumWidth: 320
    minimumHeight: 240
    visible: true
    title: qsTr("Macropad Companion")
    color: "#222222"

    function windowShowRequested() {
        if (mainWindow.visibility === Window.Hidden || mainWindow.visibility === Window.Minimized) {
            mainWindow.show();
        }

        mainWindow.raise();
        mainWindow.requestActivate();
    }

    SplitView {
        id: contentSplitter

        anchors.fill: parent
        anchors.margins: 8
        orientation: Qt.Horizontal

        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 1
            color: SplitHandle.pressed ? "#4e5946"
                                       : (SplitHandle.hovered ? "#485441" : "#3e4a36")

            containmentMask: Item {
                x: (handleDelegate.width - width) / 2
                width: 10
                height: contentSplitter.height
            }
        }

        Item {
            SplitView.minimumWidth: 120
            SplitView.preferredHeight: 200
            SplitView.maximumWidth: 300
            width: 260

            DebugControlsView {
                anchors.fill: parent
                anchors.rightMargin: 8
            }
        }

        Item {
            Rectangle {
                anchors.fill: parent
                anchors.leftMargin: 8
                color: "#333333"
                radius: 8

                CText {
                    anchors.centerIn: parent
                    width: parent.width

                    label: "The other view"
                    fontSize: 32
                    hAlign: Text.AlignHCenter
                }
            }
        }
    }
}
