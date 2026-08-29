import QtQuick
import QtQuick.Controls.Basic

import Macropad.AppContext 1.0


ApplicationWindow {
    id: mainWindow

    property bool lastSizeRestored: false
    readonly property int minWidth: 900
    readonly property int minHeight: 540

    AppController {
        id: controller
        appContext: AppContext
    }

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
        var size = controller.windowSize;
        width = size.width;
        height = size.height;
        lastSizeRestored = true;
    }

    function saveWindowSize() {
        if (lastSizeRestored) {
            controller.setWindowSize(Qt.size(width, height));
        }
    }

    Connections {
        target: controller
        function onShowWindowRequested() {
            if (mainWindow.visibility === Window.Hidden || mainWindow.visibility === Window.Minimized) {
                mainWindow.show();
            }

            mainWindow.raise();
            mainWindow.requestActivate();       
        }
    }

    AppStack {
        id: appStack

        anchors.fill: parent

        controller: controller
    }
}
