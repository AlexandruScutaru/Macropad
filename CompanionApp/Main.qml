import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


ApplicationWindow {
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
            id: devHelperViewContainer
            objectName: "devHelperViewContainer"

            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 300
            width: 280
            visible: isDebugInstance
        }

        Item {
            id: deviceViewContainer
            objectName: "deviceViewContainer"
        }
    }
}
