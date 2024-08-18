import QtQuick 2.15
import QtQuick.Controls.Basic 2.15


Text {
    id: txt

    property string label
    property string toolTipText
    property int fontSize: 12
    property int hAlign: Text.AlignLeft
    property int vAlign: Text.AlignVCenter
    property int wrap: Text.NoWrap

    horizontalAlignment: hAlign
    verticalAlignment: vAlign

    text: label
    color: "#d9e7cb"
    font.pointSize: fontSize
    elide: Text.ElideRight
    wrapMode: wrap

    Timer {
        id: hoverDelayTimer

        interval: 600
        repeat: false

        onTriggered: {
            if (mouseArea.isHovered && toolTipText.length > 0) {
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
            if (toolTipText.length === 0) {
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
        text: toolTipText
        visible: false
        delay: 0
    }
}
