import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic


ApplicationWindow {
    id: mainWindow

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
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
            visible: false
        }

        Item {
            id: deviceViewContainer
            objectName: "deviceViewContainer"
        }
    }

    Popup {
        id: devHelperOverlay

        margins: 0
        padding: 0
        modal: false
        dim: false
        focus: true
        closePolicy: Popup.NoAutoClose
        visible: isDebugInstance
        parent: Overlay.overlay

        x: 16
        y: mainWindow.height - height - 16

        width: expandDevButton.width
        height: expandDevButton.height

        background: Rectangle {
            color: "transparent"
            border.width: 0
        }

        CIconButton {
            id: expandDevButton

            iconName: "qrc:///resources/sliders_icon.png"
            toolTipText: "Expand dev helper view"
            buttonSize: 32

            anchors.centerIn: parent

            onButtonClicked: {
                devHelperViewContainer.visible = !devHelperViewContainer.visible;
            }
        }
    }
}
