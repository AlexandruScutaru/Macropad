pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls

Rectangle {
    id: settings

    anchors.fill: parent
    anchors.margins: 0

    color: Theme.backgroundPrimary
    border.color: Theme.border
    border.width: 1
    radius:8

    signal closeRequested
    property string currentSelection: settingsTabsModel.get(0).name

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
                    model: settingsTabsModel

                    CTabButton {
                        Layout.fillWidth: true

                        required property string name
                        //required property string viewSource

                        label: name
                        checked: name === settings.currentSelection

                        onButtonClicked: {
                            if (settings.currentSelection === name) {
                                return;
                            }

                            settings.currentSelection = name;
                            //TODO: settingsStack.replace(viewSource);
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

        ColumnLayout {
            CText {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: 12
                Layout.leftMargin: 20

                label: settings.currentSelection
                fontSize: 20
                hAlign: Text.AlignLeft
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 20
                Layout.bottomMargin: 12

                StackView {
                    id: settingsStack

                    anchors.fill: parent
                    anchors.margins: 12

                    clip: true
                    //initialItem: settingsTabsModel.get(0).viewSource
                }
            }
        }

        CIconButton {
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: 12
            Layout.rightMargin: 12

            iconName: "qrc:///resources/close.svg"
            toolTipText: qsTr("Close")
            iconAnimationType: CIcon.AnimationType.Scale

            onButtonClicked: {
                settings.closeRequested();
            }
        }
    }

    ListModel {
        id: settingsTabsModel

        ListElement { name: "General" }
        ListElement { name: "Sliders" }
        ListElement { name: "About" }
    }
}
