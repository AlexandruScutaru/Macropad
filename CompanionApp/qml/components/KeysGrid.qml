pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: keys

    signal actionAssigned(key: int, actionName: string)
    signal keySelected(key: int)
    signal keyTriggered(key: int)

    property alias model: keysRepeater.model
    property color outlineColor: "transparent"

    property int selectedKey: -1
    property real aspectRatio: 1.0

    implicitWidth: height * aspectRatio
    implicitHeight: width / aspectRatio

    Rectangle {
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
                    required property string keyActionName
                    required property string keyActionDisplayName
                    required property string keyActionIcon

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    isRound: keyIsRound
                    actionName: keyActionName
                    actionDisplayName: keyActionDisplayName
                    actionIconName: keyActionIcon

                    selected: isKeySelected()

                    onActionAssigned:(actionName) => {
                        keys.actionAssigned(index, actionName);
                        keys.selectedKey = -1;
                        selectKey();
                    }

                    onClicked: selectKey()

                    onDoubleClicked: {
                        keys.keyTriggered(index);
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
}
