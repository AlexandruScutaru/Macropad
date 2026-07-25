pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls
import "."

Item {
    id: appStackView

    anchors.fill: parent

    // required property MainController mainController

    function deviceConnectTryAgainClicked() {
        stack.clear();
        stack.push(loadingView);
        MacroPad.connectToDevice();
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

                onNavTabButtonClicked: (tabName) => {
                    switch (tabName) {
                        case navBarModel.keypadTabName:
                            stack.currentItem.currentIndex = 0;
                            break;
                        case navBarModel.slidersTabName:
                            stack.currentItem.currentIndex = 1;
                            break;
                        case navBarModel.settingsTabName:
                            settingsPopup.visible = true;
                            break;
                        default:
                            console.log("Uhm!? Oops...");
                    }
                }

                onNavBarExpandedChanged: (expanded) => {
                    MacroPad.saveNavBarExpanded(expanded);
                }

                Component.onCompleted: {
                    navBar.expanded = MacroPad.navBarExpanded();
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
                    MacroPad.connectToDevice();
                }

                Connections {
                    target: MacroPad
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

    Popup {
        id: settingsPopup

        parent: Overlay.overlay
        anchors.centerIn: parent
        width: parent.width * 0.7
        height: parent.height * 0.8

        visible: false
        modal: true
        closePolicy: Popup.CloseOnEscape

        background: Rectangle {
            color: "transparent"
        }

        Overlay.modal: Rectangle {
            color: Theme.backgroundBackdrop
        }

        contentItem: Item {
            anchors.fill: parent

            Loader {
                id: settingsLoader
                anchors.fill: parent

                active: false
                sourceComponent: settingsView
            }

            Connections {
                target: settingsLoader.item
                function onCloseRequested() {
                    settingsLoader.active = false;
                    settingsPopup.visible = false;
                }
            }
        }

        onVisibleChanged: {
            focus = visible;
            settingsLoader.active = visible;
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
        id: macropadView
        StackLayout {
            id: stackLayout
            currentIndex: 0

            KeypadView {}
            SlidersView {}
        }
    }

    Component {
        id: settingsView
        SettingsView {}
    }
}
