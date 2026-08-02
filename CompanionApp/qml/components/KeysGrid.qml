pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls.Basic

import Controls

Control {
    id: keys

    activeFocusOnTab: false
    focusPolicy: Qt.StrongFocus
    focus: false

    signal actionAssigned(key: int, actionId: string)
    signal keySelected(key: int)
    signal keyTriggered(key: int)

    property alias model: keysRepeater.model
    property color outlineColor: "transparent"

    property int selectedKey: -1
    property real aspectRatio: 1.0

    implicitWidth: height * aspectRatio
    implicitHeight: width / aspectRatio

    Keys.onPressed: (event) => {
        if (focus && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
            if (keysRepeater.count !== 0) {
                keysRepeater.itemAt(0).forceActiveFocus(Qt.TabFocusReason);
            }

            event.accepted = true;
        }
    }

    contentItem: Rectangle {
        id: gridContainer

        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        implicitWidth: height * keys.aspectRatio

        color: Theme.backgroundPrimary
        radius: 8
        layer.enabled: true
        layer.effect: Glow {
            samples: 25
            radius: 12
            spread: 0.3
            color: keys.outlineColor
            transparentBorder: true
            cached: true
        }

        GridLayout {
            id: gridLayout

            anchors.fill: parent
            anchors.margins: 12
            rowSpacing: 12
            columnSpacing: 12

            columns: 3
            rows: 3

            Repeater {
                id: keysRepeater

                Key {
                    id: keyCell

                    required property int index
                    required property bool keyIsRound
                    required property string keyActionId
                    required property string keyActionDisplayName
                    required property string keyActionIcon

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.row: Math.floor(index / gridLayout.columns)
                    Layout.column: index % gridLayout.columns

                    KeyNavigation.left: index % gridLayout.columns === 0 ? null : keysRepeater.itemAt(index - 1)
                    KeyNavigation.right: index % gridLayout.columns === gridLayout.columns - 1 ? null : keysRepeater.itemAt(index + 1)
                    KeyNavigation.up: index < gridLayout.columns ? null : keysRepeater.itemAt(index - gridLayout.columns)
                    KeyNavigation.down: index + gridLayout.columns >= keysRepeater.count ? null : keysRepeater.itemAt(index + gridLayout.columns)

                    isRound: keyIsRound
                    actionId: keyActionId
                    actionDisplayName: keyActionDisplayName
                    actionIconName: keyActionIcon

                    selected: isKeySelected()

                    onActionAssigned:(actionId) => {
                        keys.actionAssigned(index, actionId);
                        keys.selectedKey = -1;
                        selectKey();
                    }

                    onClicked: selectKey()

                    onDoubleClicked: {
                        keys.keyTriggered(index);
                    }

                    Keys.onPressed: (event) => {
                        if (event.key === Qt.Key_Escape) {
                            keys.forceActiveFocus(Qt.BacktabFocusReason);
                            event.accepted = true;
                        }
                    }

                    function selectKey() {
                        if (isKeySelected()) {
                            return;
                        }

                        keys.selectedKey = index;
                        keys.keySelected(index);
                    }

                    function isKeySelected(): bool {
                        return keys.selectedKey === index;
                    }
                }
            }
        }
    }

    CFocusOutline {
        target: keys
        anchors.fill: keys
        radius: 8
    }
}
