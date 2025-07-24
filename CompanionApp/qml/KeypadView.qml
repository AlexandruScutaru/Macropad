pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls
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

                    SplitView.minimumHeight: actionAssignLayout.anchors.topMargin + keypadKeys.minSize + actionAssignLayout.spacing + layerPagination.height + actionAssignLayout.anchors.bottomMargin
                    SplitView.fillHeight: true

                    ColumnLayout {
                        id: actionAssignLayout

                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 20

                        KeypadKeys {
                            id: keypadKeys

                            readonly property int minSize: 150
                            readonly property int maxSize: 350

                            Layout.minimumHeight: minSize
                            Layout.minimumWidth: minSize
                            Layout.maximumHeight: maxSize
                            Layout.maximumWidth: maxSize
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignCenter

                            outlineColor: "transparent"

                            onKeySelected: (row, col) => {}
                            onKeyTriggered: (row, col) => {}
                        }

                        LayerPagination {
                            id: layerPagination

                            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                            // temp data for time being
                            readonly property var colors: ["red", "yellow", "magenta", "green", "cyan"];

                            pageCount: 5
                            currentPage: 0

                            Component.onCompleted: {
                                keypadKeys.outlineColor = colors[currentPage];
                            }

                            onCurrentPageChanged: { keypadKeys.outlineColor = colors[currentPage]; }
                            onPageChanged: (page) => { currentPage = page; }
                        }
                    }
                }

                Item {
                    id: actionConfigArea
                    SplitView.minimumHeight: actionSplitter.height * 0.3

                    CVerticalScrollView {
                        id: scrollView

                        anchors.fill: parent
                        anchors.margins: 0
                    }
                }
            }
        }

        Rectangle {
            id: actionsSidePanel

            Layout.fillHeight: true
            Layout.preferredWidth: 340

            color: Theme.backgroundSecondary

            ActionsList {
                id: actionsList
                anchors.fill: parent
            }
        }
    }
}
