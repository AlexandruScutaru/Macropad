pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import "."

Item {
    id: keypadView

    property KeypadController keypadController

    Component.onCompleted: {
        keypadController = MacroPad.getKeypadController();
        actionsList.actionsModel = keypadController.getActionSectionsListModel();
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: Theme.backgroundPrimary

            SplitView {
                id: actionSplitter

                anchors.fill: parent
                orientation: Qt.Vertical

                handle: Rectangle {
                    id: handleDelegate

                    implicitWidth: 4
                    implicitHeight: 4

                    color: getColor()

                    function getColor() {
                        if (SplitHandle.pressed) return Theme.buttonSecondaryPressed;
                        if (SplitHandle.hovered) return Theme.buttonSecondaryHovered;
                        return Theme.backgroundTertiary;
                    }

                    containmentMask: Item {
                        y: (handleDelegate.height - height) / 2
                        width: actionSplitter.width
                        height: 12
                    }
                }

                Item {
                    id: keypadActionAssignArea
                    SplitView.minimumHeight: actionSplitter.height * 0.3
                    SplitView.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12

                        Item {
                            Layout.fillHeight: true
                        }

                        KeypadKeys {
                            id: keypadKeys

                            Layout.minimumHeight: 100
                            Layout.minimumWidth: 100
                            Layout.maximumHeight: 350
                            Layout.maximumWidth: 350
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignCenter

                            outlineColor: "red"

                            onKeySelected: (row, col) => {}
                            onKeyTriggered: (row, col) => {}
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        LayerPagination {
                            id: layerPagination

                            Layout.alignment: Qt.AlignHCenter
                            Layout.topMargin: 12
                            Layout.bottomMargin: 8

                            pageCount: 5
                            currentPage: 0

                            onPageChanged: (page) => {
                                const colors = ["red", "yellow", "magenta", "green", "cyan"];
                                keypadKeys.outlineColor = colors[page];
                            }
                        }
                    }
                }

                Item {
                    id: actionConfigArea
                    SplitView.minimumHeight: actionSplitter.height * 0.3
                }
            }
        }

        Rectangle {
            id: actionsSidePanel

            Layout.fillHeight: true
            Layout.preferredWidth: 280

            color: Theme.backgroundSecondary

            ActionsList {
                id: actionsList
                anchors.fill: parent
            }
        }
    }
}
