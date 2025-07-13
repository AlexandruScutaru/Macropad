import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    id: appStackView

    required property MainController mainController

    anchors.fill: parent

    function deviceConnectTryAgainClicked() {
        stack.clear();
        stack.push(loadingView);
        mainController.connectToDevice();
    }

    StackView {
        id: stack

        anchors.fill: parent
        clip: true
        initialItem: loadingView

        pushEnter: animNone
        popEnter: animNone
        replaceEnter: animNone
        pushExit: animNone
        popExit: animNone
        replaceExit: animNone

        Component.onCompleted: {
            appStackView.mainController.connectToDevice();
        }

        Connections {
            target: appStackView.mainController
            function onDeviceConnected() {
                stack.replace(dashboardView);
            }

            function onDeviceNotFound() {
                stack.replace(notConnectedView);
            }
        }

        Component {
            id: loadingView
            Item {
                CBusyIndicator {
                    anchors.centerIn: parent
                    width: 64
                    height: 64
                }
            }
        }

        Component {
            id: notConnectedView
            NotConnectedView {}
        }

        Component {
            id: dashboardView
            DashboardView {}
        }

        Transition {
            id: animNone
        }

        Transition {
            id: animFadeIn
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 100
            }
        }

        Transition {
            id: animFadeOut
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 100
            }
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
