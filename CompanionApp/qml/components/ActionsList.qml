import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls

Item {
    id: actionsList

    required property AvailableActionsController controller

    CVerticalScrollView {
        id: scrollView

        anchors.fill: parent
        anchors.margins: 0

        ColumnLayout {
            width: scrollView.width - 12
            spacing: 0

            Repeater {
                id: sectionRepeater

                model: actionsList.controller.model

                Column {
                    id: actionGroup

                    required property string name
                    required property string iconName
                    required property var actionList

                    Layout.fillWidth: true

                    CCollapseableHeader {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        iconName: "qrc:///resources/%1".arg(actionGroup.iconName)
                        iconSize: 20
                        text: actionGroup.name
                        expanded: actionGroupWrapper.shown

                        onClicked: actionGroupWrapper.shown = !actionGroupWrapper.shown
                    }

                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        height: 2
                        color: Theme.backgroundPrimary
                    }

                    Pane {
                        id: actionGroupWrapper

                        property bool shown: true

                        anchors.margins: 0
                        anchors.leftMargin: 8
                        padding: 0
                        anchors.left: parent.left
                        anchors.right: parent.right
                        clip: true
                        height: shown ? actionGroupColumn.height : 0
                        visible: height > 0

                        background: Rectangle {
                            color: Theme.backgroundPrimary
                        }

                        Behavior on height {
                            NumberAnimation {
                                easing.type: Easing.InOutQuad
                                duration: 200
                            }
                        }

                        Column {
                            id: actionGroupColumn

                            anchors.left: parent.left
                            anchors.right: parent.right

                            Repeater {
                                model: actionGroup.actionList

                                CDragableItem {
                                    required property var modelData

                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    mimePayload: `{
                                        "actionId": "${modelData.id}"
                                    }`
                                    label: modelData.displayName
                                    toolTipText: modelData.toolTip
                                    iconName: "qrc:///resources/%1".arg(modelData.iconName)
                                    leftMargin: 12 + 16 + 12 - 8 // align to header item icon: leftMargin + chevron_arrow + spacing - pane leftMargin
                                }
                            }
                        }
                    }
                }
            }

            // anchors.bottomMargin on the ColumnLayout didn't seem to do anything
            // using this to push the content up a bit but not the scroll bar
            Item {
                Layout.preferredHeight: 8
            }
        }
    }
}
