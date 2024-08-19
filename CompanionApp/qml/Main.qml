import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls 1.0

ApplicationWindow {
    id: mainWindow

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("Macropad Companion")
    color: "#222222"

    onWidthChanged: {
        MacroPad.saveWindowSize(width, height);
    }

    onHeightChanged: {
        MacroPad.saveWindowSize(width, height);
    }

    Component.onCompleted: {
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
        id: deviceStackViewContainer

        anchors.fill: parent
        objectName: "deviceStackViewContainer"
    }
}
