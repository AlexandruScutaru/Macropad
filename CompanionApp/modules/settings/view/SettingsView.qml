pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Components
import Macropad.AppContext 1.0


Rectangle {
    id: settings

    property string currentSelection

    signal closeRequested

    SettingsController {
        id: controller
        appContext: AppContext
    }

    anchors.fill: parent
    anchors.margins: 0

    color: Theme.backgroundPrimary
    border.color: Theme.border
    border.width: 1
    radius:8

    RowLayout {
        anchors.fill: parent

        Rectangle {
            id: settingsTabs

            Layout.margins: 1
            Layout.rightMargin: 0
            Layout.preferredWidth: 180
            Layout.fillHeight: true

            topLeftRadius: 8
            bottomLeftRadius: 8
            color: Theme.backgroundSecondary

            ColumnLayout {
                anchors.fill: parent
                anchors {
                    topMargin: 12
                    leftMargin: 12
                    bottomMargin: 12
                    rightMargin: 8
                }

                Repeater {
                    id: tabButtonsRepeater
                    model: controller.tabsListModel

                    CTabButton {
                        Layout.fillWidth: true

                        required property string name
                        required property string url

                        label: name
                        checked: name === settings.currentSelection

                        onButtonClicked: {
                            if (settings.currentSelection === name) {
                                return;
                            }

                            settings.currentSelection = name;
                            settingsStack.replace(url, {
                                controller: controller
                            });
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

        ColumnLayout {
            spacing: 0

            RowLayout {
                CText {
                    Layout.fillWidth: true
                    Layout.topMargin: 12
                    Layout.leftMargin: 12

                    label: settings.currentSelection
                    fontSize: 14
                    fontWeight: Font.Normal
                    hAlign: Text.AlignLeft
                }

                CIconButton {
                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                    Layout.topMargin: 12
                    Layout.rightMargin: 12

                    iconName: "qrc:///resources/icons/close.svg"
                    toolTipText: qsTr("Close")
                    iconAnimationType: CIcon.AnimationType.Scale

                    onButtonClicked: {
                        settings.closeRequested();
                    }
                }
            }

            CGradientSeparator {
                Layout.fillWidth: true
                Layout.topMargin: 8
            }

            StackView {
                id: settingsStack

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 20

                pushEnter: animNone
                popEnter: animNone
                replaceEnter: animNone
                pushExit: animNone
                popExit: animNone
                replaceExit: animNone

                Component.onCompleted: {
                    let firstTabButton = tabButtonsRepeater.itemAt(0);
                    if (!firstTabButton) {
                        return;
                    }

                    settings.currentSelection = firstTabButton.name;

                    settingsStack.push(firstTabButton.url, {
                        controller: controller
                    });
                }

                Transition {
                    id: animNone
                }
            }
        }
    }
}
