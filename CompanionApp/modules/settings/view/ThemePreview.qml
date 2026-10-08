pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: preview

    required property variant model

    implicitWidth: 200
    implicitHeight: 150

    Rectangle {
        id: miniApp

        anchors.fill: parent

        color: preview.model.backgroundPrimary
        radius: 6
        z: 1

        Rectangle {
            id: miniNavBar

            width: parent.width * 0.2
            height: parent.height
            anchors.left: parent.left

            topLeftRadius: parent.topLeftRadius
            bottomLeftRadius: parent.bottomLeftRadius

            color: preview.model.backgroundSecondary

            Rectangle {
                id: miniNavTabSelected

                width: parent.width * 0.8
                height: width * 0.5
                anchors.top: parent.top
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter

                color: preview.model.buttonSecondaryHovered
                radius: 4

                Rectangle {
                    width: parent.width * 0.6
                    height: parent.height * 0.2
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    anchors.verticalCenter: parent.verticalCenter

                    color: preview.model.textPrimary
                    radius: height / 2
                }
            }

            Rectangle {
                id: miniNavTabUnselected

                width: parent.width * 0.8
                height: width * 0.5
                anchors.top: miniNavTabSelected.bottom
                anchors.topMargin: 4
                anchors.horizontalCenter: parent.horizontalCenter

                color: preview.model.buttonSecondaryNormal
                radius: 4

                Rectangle {
                    width: parent.width * 0.6
                    height: parent.height * 0.2
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    anchors.verticalCenter: parent.verticalCenter

                    color: preview.model.textSecondary
                    radius: height / 2
                }
            }
        }

        ColumnLayout {
            id: miniCentralPanel

            height: miniApp.height
            width: miniApp.width - miniNavBar.width - miniActionsContainer.width
            anchors.left: miniNavBar.right
            anchors.top: miniApp.top

            spacing: 6

            Rectangle {
                Layout.topMargin: 12
                Layout.preferredWidth: miniCentralPanel.width * 0.6
                Layout.preferredHeight: miniCentralPanel.width * 0.6
                Layout.alignment: Qt.AlignHCenter

                color: preview.model.backgroundTertiary

                radius: 4
            }

            Item {
                Layout.fillHeight: true
            }

            Rectangle {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height * 0.01
                Layout.alignment: Qt.AlignHCenter

                color: preview.model.accentPrimaryNormal
            }

            Item {
                id: formRow

                Layout.preferredWidth: miniCentralPanel.width * 0.9
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: miniCentralPanel.height * 0.08

                RowLayout {
                    anchors.fill: parent

                    spacing: 4

                    Rectangle {
                        id: label

                        Layout.preferredWidth: formRow.width * 0.2
                        Layout.preferredHeight: formRow.height * 0.5

                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                        color: preview.model.textPrimary
                        radius: height / 2
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        id: input

                        Layout.preferredWidth: formRow.width * 0.5
                        Layout.fillHeight: true

                        color: preview.model.backgroundTertiary
                        radius: 4
                    }

                    Rectangle {
                        id: button

                        Layout.preferredWidth: formRow.height
                        Layout.fillHeight: true

                        color: preview.model.buttonSecondaryHovered
                        radius: 4
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: parent.width * 0.5
                Layout.preferredHeight: parent.height * 0.1
                Layout.bottomMargin: 8
                Layout.rightMargin: 6
                Layout.alignment: Qt.AlignRight

                color: preview.model.buttonPrimaryNormal

                radius: height / 2
            }
        }

        Rectangle {
            id: miniActionsContainer

            width: parent.width * 0.3
            height: parent.height
            anchors.right: parent.right

            topRightRadius: parent.topRightRadius
            bottomRightRadius: parent.bottomRightRadius

            color: preview.model.backgroundSecondary

            Rectangle {
                id: miniActionGroup

                width: parent.width
                height: parent.height * 0.1
                anchors.right: parent.right
                anchors.top: parent.top

                topRightRadius: parent.topRightRadius

                color: preview.model.buttonSecondaryNormal

                Rectangle {
                    width: parent.width * 0.8
                    height: parent.height * 0.2
                    anchors.centerIn: parent

                    color: preview.model.textSecondary
                    radius: height / 2
                }
            }

            Rectangle {
                id: miniActionsList

                width: parent.width * 0.95
                height: parent.height * 0.4
                anchors.right: parent.right
                anchors.top: miniActionGroup.bottom

                color: preview.model.backgroundPrimary

                ColumnLayout {
                    anchors.fill: parent
                    anchors.topMargin: 4
                    anchors.bottomMargin: 4

                    Repeater {
                        model: 5

                        Rectangle {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: miniActionsList.width * 0.7
                            Layout.preferredHeight: miniActionHovered.height * 0.2

                            color: preview.model.textSecondary
                            radius: height / 2
                        }
                    }
                }
            }

            Rectangle {
                id: miniActionHovered

                width: miniActionsList.width
                height: parent.height * 0.1
                anchors.right: parent.right
                anchors.top: miniActionsList.bottom

                color: preview.model.buttonSecondaryHovered

                Rectangle {
                    width: parent.width * 0.7
                    height: parent.height * 0.2
                    anchors.centerIn: parent

                    color: preview.model.textPrimary
                    radius: height / 2
                }
            }
        }

        RectangularShadow {
            anchors.fill: miniApp

            color: '#30000000'

            radius: miniApp.radius
            blur: 20
            spread: 6
            offset: Qt.point(0, 0)
            z: -1
        }
    }
}
