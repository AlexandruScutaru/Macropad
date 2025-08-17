pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls
import "."

Item {
    id: keypadView

    property KeypadController keypadController
    property var layersModel

    property int currentLayerIndex: 0
    property int currentKey: -1

    Component.onCompleted: {
        keypadController = MacroPad.getKeypadController();
        actionsList.actionsModel = keypadController.getActionSectionsListModel();
        layersModel = keypadController.getLayerListModel();
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

                            model: keypadView.layersModel.getRow(keypadView.currentLayerIndex)

                            onKeyActionAssigned: (key, actionId) => {
                                keypadView.keypadController.assignAction(keypadView.currentLayerIndex, key, actionId);
                            }

                            onKeySelected: (key, actionId) => {
                                keypadView.currentKey = key;
                            }

                            onKeyTriggered: (key) => {}
                        }

                        LayerPagination {
                            id: layerPagination

                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                            pageCount: keypadView.layersModel.count
                            currentPage: 0

                            onPageChanged: (page) => {
                                keypadView.currentLayerIndex = page;
                            }
                        }
                    }
                }

                Item {
                    id: actionConfigArea
                    SplitView.minimumHeight: actionSplitter.height * 0.3

                    KeyConfig {
                        id: keyConfig

                        anchors.fill: parent
                        anchors.margins: 0

                        model: keypadView.currentKey >= 0 ? keypadView.layersModel.getRow(keypadView.currentLayerIndex).keysList.getRow(keypadView.currentKey) : undefined
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
