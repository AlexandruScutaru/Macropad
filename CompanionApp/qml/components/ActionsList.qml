import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls

Item {
    id: actionsList

    property alias actionsModel: sectionRepeater.model

    ScrollView {
        id: scrollView

        anchors.fill: parent
        anchors.margins: 0

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            width: scrollView.width
            spacing: 0

            Repeater {
                id: sectionRepeater

                Column {
                    id: actionGroup

                    required property string sectionName
                    required property string iconName
                    required property var actionList

                    Layout.fillWidth: true

                    CCollapseableHeader {
                        anchors.left: parent.left
                        anchors.right: parent.right

                        iconName: "qrc:///resources/%1".arg(actionGroup.iconName)
                        iconSize: 20
                        text: actionGroup.sectionName
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

                        property bool shown: false

                        anchors.margins: 0
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
                                        "iconName": "${modelData.iconName}",
                                        "actionName": "${modelData.actionName}",
                                        "actionId": "${modelData.actionId}"
                                    }`
                                    label: modelData.actionName
                                    iconName: "qrc:///resources/%1".arg(modelData.iconName)
                                    leftMargin: 12 + 16 + 12 // header item: leftMargin + chevron_icon + spacing
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
