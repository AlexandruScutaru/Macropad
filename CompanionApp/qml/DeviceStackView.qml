import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    id: root

    anchors.fill: parent
    anchors.margins: 16

    required property MainController mainController

    StackView {
        id: stack

        anchors.fill: parent

        clip: true

        Component.onCompleted: {
            mainController.openLastDevice();
        }

        Connections {
            target: mainController
            function onDeviceConnected() {
                stack.replace(dashboardView);
            }

            function onNoDeviceSaved() {
                stack.replace(connectView);
            }
        }

        Component {
            id: connectView
            DeviceConnectView {}
        }

        Component {
            id: dashboardView
            DeviceDashboardView {}
        }

        Transition {
            id: animEnterLeftToRight
            PropertyAnimation {
                property: "x"
                from: -stack.width
                to: 0
                duration: 250
                easing.type: Easing.InQuad
            }
        }

        Transition {
            id: animExitLeftToRight
            PropertyAnimation {
                property: "x"
                from: 0
                to: stack.width
                duration: 250
                easing.type: Easing.OutQuad
            }
        }

        Transition {
            id: animEnterRightToLeft
            PropertyAnimation {
                property: "x"
                from: stack.width
                to: 0
                duration: 250
                easing.type: Easing.InQuad
            }
        }

        Transition {
            id: animExitRightToLeft
            PropertyAnimation {
                property: "x"
                from: 0
                to: -stack.width
                duration: 250
                easing.type: Easing.OutQuad
            }
        }
    }
}
