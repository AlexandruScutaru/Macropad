pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import "."

Item {
    id: keypadView

    property KeypadModule keypadModule

    Component.onCompleted: {
        keypadModule = MacroPad.getKeypadModule();
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
                        if (SplitHandle.pressed) return Theme.accentPrimaryNormal;
                        if (SplitHandle.hovered) return Theme.accentPrimaryHovered;
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

                    SplitView.minimumHeight: 250
                    SplitView.fillHeight: true

                    KeypadLayers {
                        id: keypadLayer

                        anchors.fill: parent
                        anchors.margins: 0

                        controller: keypadView.keypadModule.getKeypadController()
                    }
                }

                Item {
                    id: actionConfigArea
                    SplitView.minimumHeight: actionSplitter.height * 0.3

                    KeyConfig {
                        id: keyConfig

                        width: parent.width > 600 ? 600 : parent.width
                        height: parent.height
                        anchors.centerIn: parent
                        anchors.margins: 0

                        controller: keypadView.keypadModule.getActionConfigController()
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

                controller: keypadView.keypadModule.getAvailableActionsController()
            }
        }
    }
}
