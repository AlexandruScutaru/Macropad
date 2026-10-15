import QtQuick
import QtQuick.Controls.Basic

import Components

Cell {
    id: encoder

    property string actionId: ""
    property string actionDisplayName: ""
    property string actionIconName: ""

    mouseArea.preventStealing: true

    Keys.onPressed: (event) => {
        if (!encoder.selected) {
            event.accepted = false;
            return;
        }

        if (event.key === Qt.Key_PageUp) {
            customDial.stepAngle(1)
            event.accepted = true
        } else if (event.key === Qt.Key_PageDown) {
            customDial.stepAngle(-1)
            event.accepted = true
        }
    }

    contentItem: Item {
        id: customDial

        implicitWidth: 140
        implicitHeight: 140

        property real value: 0
        property real from: 0
        property real to: 360
        property real angle: 0
        property bool snapMode: true
        property real stepSize: 12

        property real lastX: 0
        property real dragAngle: 0

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: encoder.getBgColor()
            border.color: encoder.getBorderColor()
            border.width: 2
        }

        Rectangle {
            width: customDial.width * 0.1
            height: width
            radius: width / 2
            color: encoder.getTextColor()

            x: customDial.width / 2 - width / 2 + Math.cos((customDial.angle - 90) * Math.PI / 180) * (customDial.width / 2 * 0.75)
            y: customDial.height / 2 - height / 2 + Math.sin((customDial.angle - 90) * Math.PI / 180) * (customDial.height / 2 * 0.75)
        }

        Connections {
            target: encoder.mouseArea

            function onPositionChanged(mouse: MouseEvent) {
                if (encoder.mouseArea.pressed && encoder.selected) {
                    let deltaX = mouse.x - customDial.lastX;
                    customDial.lastX = mouse.x;
                    customDial.dragAngle += deltaX;

                    let nextAngle = customDial.dragAngle;
                    if (customDial.snapMode && customDial.stepSize > 0) {
                        nextAngle = Math.round(nextAngle / customDial.stepSize) * customDial.stepSize;
                    }

                    customDial.setAngle(nextAngle);
                }
            }

            function onPressed(mouse: MouseEvent) {
                customDial.lastX = mouse.x;
                customDial.dragAngle = customDial.angle;
            }
        }

        function setAngle(nextAngle) {
            angle = ((nextAngle % 360) + 360) % 360
            value = from + (angle / 360.0) * (to - from)
        }

        function stepAngle(direction) {
            if (stepSize <= 0) {
                return
            }

            dragAngle += direction * stepSize
            let nextAngle = dragAngle
            if (snapMode) {
                nextAngle = Math.round(nextAngle / stepSize) * stepSize
            }
            setAngle(nextAngle)
        }
    }

    CFocusOutline {
        target: encoder
        anchors.fill: parent
        radius: encoder.width / 2
    }

    ToolTip {
        text: encoder.actionDisplayName
        visible: encoder.mouseArea.containsMouse && encoder.actionId >= 0 && encoder.actionDisplayName.length > 0
        delay: 600
    }
}
