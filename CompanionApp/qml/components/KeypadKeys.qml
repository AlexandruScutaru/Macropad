pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id: keys

    signal keySelected(row: int, col: int)
    signal keyTriggered(row: int, col: int)

    property alias outlineColor: keypadContainer.glowColor
    property point selectedKey: Qt.point(-1, -1)
    property real aspectRatio: 1.0

    implicitWidth: height * aspectRatio
    implicitHeight: width / aspectRatio

    Rectangle {
        id: keypadContainer

        property color glowColor: "green"
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
            color: keypadContainer.glowColor
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
                model: keypadKeysModel

                Key {
                    id: keyCell

                    required property int row
                    required property int column
                    required property bool round

                    Layout.row: row
                    Layout.column: column
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    isRound: round

                    selected: isKeySelected()

                    onActionAssigned: selectKey()
                    onClicked: selectKey()

                    onDoubleClicked: {
                        keys.keyTriggered(row, column)
                    }

                    function selectKey() {
                        if (isKeySelected()) {
                            return;
                        }

                        keys.selectedKey = Qt.point(column, row);
                        keys.keySelected(row, column)
                    }

                    function isKeySelected(): bool {
                        // y as in (vertical) rows and x for (horizontal) columns
                        return row === keys.selectedKey.y && column === keys.selectedKey.x;
                    }
                }
            }
        }
    }

    ListModel {
        id: keypadKeysModel

        ListElement { row: 0; column: 0; round: true }
        ListElement { row: 0; column: 1; round: false }
        ListElement { row: 0; column: 2; round: false }
        ListElement { row: 1; column: 0; round: false }
        ListElement { row: 1; column: 1; round: false }
        ListElement { row: 1; column: 2; round: false }
        ListElement { row: 2; column: 0; round: false }
        ListElement { row: 2; column: 1; round: false }
        ListElement { row: 2; column: 2; round: false }
    }
}
