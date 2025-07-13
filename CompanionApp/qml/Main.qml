import QtQuick
import QtQuick.Controls.Basic


ApplicationWindow {
    id: mainWindow

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Macropad Companion")
    color: Theme.backgroundPrimary;

    onWidthChanged: {
        MacroPad.saveWindowSize(width, height);
    }

    onHeightChanged: {
        MacroPad.saveWindowSize(width, height);
    }

    Component.onCompleted: {
        color = Theme.backgroundPrimary;
        var size = MacroPad.windowSize();
        width = size.width;
        height = size.height;
    }

    function windowShowRequested() {
        if (mainWindow.visibility === Window.Hidden || mainWindow.visibility === Window.Minimized) {
            mainWindow.show();
        }

        mainWindow.raise();
        mainWindow.requestActivate();
    }

    Item {
        id: appStackViewContainer

        anchors.fill: parent
        objectName: "appStackViewContainer"
    }
}
