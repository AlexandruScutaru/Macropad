import QtQuick
import QtQuick.Controls.Basic

ApplicationWindow {
    id: mainWindow

    property bool lastSizeRestored: false
    readonly property int minWidth: 900
    readonly property int minHeight: 540

    width: minWidth
    height: minHeight
    minimumWidth: minWidth
    minimumHeight: minHeight
    visible: true
    title: qsTr("Macropad Companion")
    color: Theme.backgroundPrimary;

    onWidthChanged: {
        saveWindowSize();
    }

    onHeightChanged: {
        saveWindowSize();
    }

    Component.onCompleted: {
        color = Theme.backgroundPrimary;
        var size = MacroPad.windowSize();
        width = size.width;
        height = size.height;
        lastSizeRestored = true;
    }

    function saveWindowSize() {
        if (lastSizeRestored) {
            MacroPad.saveWindowSize(width, height);
        }
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
