pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQml

import Components
import "."

FocusScope {
    id: appStack

    required property AppController controller

    activeFocusOnTab: true

    anchors.fill: parent

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Escape && settingsPopup.visible) {
            settingsPopup.visible = false;
            event.accepted = true;
        }
    }

    function deviceConnectTryAgainClicked() {
        stack.clear();
        stack.push(loadingView);
        controller.connectToDevice();
    }

    Connections {
        target: appStack.controller

        function onDeviceConnected() {
            stack.replace(macropadView);
        }

        function onDeviceNotFound() {
            stack.replace(notConnectedView);
        }

        function onOpenSettings() {
            settingsPopup.visible = true;
        }

        function onSwitchTab(index, name) {
            stack.currentItem.currentIndex = index;
            navBar.currentSelection = name;
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: navSidePanel

            Layout.fillHeight: true
            Layout.preferredWidth: navBar.implicitWidth

            color: Theme.backgroundSecondary

            NavigationBar {
                id: navBar
                anchors.fill: navSidePanel
                model: appStack.controller.navBarModel

                expanded: appStack.controller.navBarExpanded

                onNavTabButtonClicked: (tabName) => {
                    appStack.controller.navigationRequested(tabName);
                }

                onNavBarExpandedChanged: (expanded) => {
                    appStack.controller.navBarExpanded = expanded;
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: Theme.backgroundPrimary

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
                    appStack.controller.connectToDevice();
                }

                onCurrentItemChanged: {
                    if (stack.currentItem) {
                        stack.currentItem.forceActiveFocus();
                    }
                }

                Transition {
                    id: animNone
                }
            }
        }
    }

    SettingsPopup {
        id: settingsPopup

        anchors.centerIn: parent
        width: parent.width * 0.7
        height: parent.height * 0.8

        onOpened: settingsPopup.forceActiveFocus()
        onClosed: appStack.forceActiveFocus()
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
        id: macropadView

        StackLayout {
            id: stackLayout

            currentIndex: 0;

            KeypadView {}
            SlidersView {}
        }
    }
}
