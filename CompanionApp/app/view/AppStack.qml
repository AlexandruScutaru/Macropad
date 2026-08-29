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

    enum View {
        Keypad = 0,
        Sliders = 1
    }

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
                tabButtonsModel: navBarModel

                expanded: appStack.controller.navBarExpanded

                onNavTabButtonClicked: (tabName) => {
                    switch (tabName) {
                        case navBarModel.keypadTabName:
                            stack.currentItem.currentIndex = AppStack.View.Keypad;
                            break;
                        case navBarModel.slidersTabName:
                            stack.currentItem.currentIndex = AppStack.View.Sliders;
                            break;
                        case navBarModel.settingsTabName:
                            settingsPopup.visible = true;
                            break;
                        default:
                            console.log("Uhm!? Oops...");
                    }
                }

                onNavBarExpandedChanged: (expanded) => {
                    appStack.controller.setNavBarExpanded(expanded);
                }

                NavigationBarModel {
                    id: navBarModel

                    keypadTabEnabled: false
                    slidersTabEnabled: false
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

                Connections {
                    target: appStack.controller

                    function onDeviceConnected() {
                        navBarModel.keypadTabEnabled = true;
                        navBarModel.slidersTabEnabled = true;
                        stack.replace(macropadView);
                    }

                    function onDeviceNotFound() {
                        navBarModel.keypadTabEnabled = false;
                        navBarModel.slidersTabEnabled = false;
                        stack.replace(notConnectedView);
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

            currentIndex: AppStack.View.Keypad;

            KeypadView {}
            SlidersView {}
        }
    }
}
