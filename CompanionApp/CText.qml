import QtQuick 2.15
import QtQuick.Controls.Basic 2.15


Text {
    id: txt

    property string label
    property int fontSize: 12
    property int hAlign: Text.AlignLeft
    property int vAlign: Text.AlignVCenter

    text: label
    color: "#d9e7cb"
    font.pointSize: fontSize
    elide: Text.ElideRight
    horizontalAlignment: hAlign
    verticalAlignment: vAlign
}
