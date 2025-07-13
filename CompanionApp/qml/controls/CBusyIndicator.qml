pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

BusyIndicator {
    id: busyIndicator

    contentItem: Item {
        implicitWidth: 64
        implicitHeight: 64

        Item {
            id: content
            x: parent.width / 2 - 32
            y: parent.height / 2 - 32
            width: 64
            height: 64
            opacity: busyIndicator.running ? 1 : 0

            property int currentIndex: 0
            property bool direction: false

            Timer {
                id: timer

                interval: 100
                running: busyIndicator.running
                repeat: true

                onTriggered: {
                    content.currentIndex = content.currentIndex + 1;
                    if (content.currentIndex >= 10) {
                        content.currentIndex = 0;
                        content.direction = !content.direction;
                    }
                }
            }

            Repeater {
                id: repeater
                model: 10

                Rectangle {
                    id: dot
                    required property int index

                    x: content.width / 2 - width / 2
                    y: content.height / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5

                    function getColor() {
                        if (content.direction) {
                            return index > content.currentIndex ? Theme.backgroundTertiary : Theme.textSecondary
                        }
                        return index > content.currentIndex ? Theme.textSecondary : Theme.backgroundTertiary
                    }

                    color: getColor()
                    transform: [
                        Translate {
                            y: -Math.min(content.width, content.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: dot.index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                        }
                    ]
                }
            }
        }
    }
}
