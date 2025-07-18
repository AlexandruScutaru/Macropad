import QtQuick
import QtQuick.Controls.Basic

Text {
    id: txt

    property string label
    property int fontSize: 12
    property int hAlign: Text.AlignLeft
    property int vAlign: Text.AlignVCenter
    property int wrap: Text.NoWrap

    horizontalAlignment: hAlign
    verticalAlignment: vAlign

    text: label
    color: Theme.textPrimary
    font.pointSize: fontSize
    elide: wrap == Text.NoWrap ? Text.ElideRight : Text.ElideNone
    wrapMode: wrap

    Timer {
        id: hoverDelayTimer

        interval: 600
        repeat: false

        onTriggered: {
            if (mouseArea.isHovered && txt.truncated) {
                tooltip.visible = true;
            }
        }
    }

    MouseArea {
        id: mouseArea

        property alias isHovered: mouseArea.containsMouse

        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            if (txt.truncated === 0) {
                return;
            }

            hoverDelayTimer.start();
        }

        onExited: {
            hoverDelayTimer.stop();
            tooltip.visible = false;
        }
    }

    ToolTip {
        id: tooltip
        text: txt.text
        visible: false
        delay: 0
    }
}
