import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    id: root
    property string currentSelection: settingsEntriesModel.get(0).name

    RowLayout {
        anchors.fill: parent
        spacing: 8

        Item {
            Layout.fillHeight: true
            implicitWidth: 160

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    spacing: 8

                    CIconButton {
                        Layout.alignment: Qt.AlignHCenter

                        iconName: "qrc:///resources/back_arrow_icon.png"
                        toolTipText: qsTr("Go back")
                        buttonSize: 32

                        onButtonClicked: {
                            stack.pop()
                        }
                    }

                    CText {
                        label: qsTr("Settings")
                        fontSize: 14
                        vAlign: Text.AlignVCenter
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                Repeater {
                    id: tabButtonsRepeater
                    model: settingsEntriesModel

                    CTabButton {
                        required property string name
                        required property string viewSource

                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft

                        label: name
                        checked: name === root.currentSelection

                        onButtonClicked: {
                            if (root.currentSelection === name) {
                                return;
                            }

                            root.currentSelection = name;
                            settingsStack.replace(viewSource);
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

                label: root.currentSelection
                fontSize: 20
                hAlign: Text.AlignLeft
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "#2d2d2d"
                radius: 8

                StackView {
                    id: settingsStack

                    anchors.fill: parent
                    anchors.margins: 12

                    clip: true
                    initialItem: settingsEntriesModel.get(0).viewSource

                    replaceEnter: Transition {
                        PropertyAnimation {
                            property: "y"
                            from: settingsStack.height
                            to: 0
                            duration: 300
                            easing.type: Easing.OutCubic
                        }
                    }

                    replaceExit: Transition {
                        PropertyAnimation {
                            property: "y"
                            from: 0
                            to: -settingsStack.height
                            duration: 300
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: settingsEntriesModel

        ListElement { name: "Device";  viewSource: "/qt/qml/MacropadCompanion/DeviceSettings.qml" }
        ListElement { name: "Hotkeys"; viewSource: "/qt/qml/MacropadCompanion/HotkeysSettings.qml" }
        ListElement { name: "Sliders"; viewSource: "/qt/qml/MacropadCompanion/SlidersSettings.qml" }
    }}
