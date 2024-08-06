import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

Item {
    id: root

    property int minValue: 0
    property int maxValue: 100
    property int currentValue: 50
    property int barWidth: 4
    property string label: ""

    implicitWidth: barWidth + rowLayout.spacing + maxValueText.width

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            id: rowLayout

            Layout.alignment: Qt.AlignHCenter
            spacing: 4

            Rectangle {
                id: outerRect

                Layout.fillHeight: true
                width: barWidth
                color: "#d9e7cb"
                radius: 2

                Rectangle {
                    id: innerRect

                    height: Math.min(((root.currentValue - root.minValue) / (root.maxValue - root.minValue)) * outerRect.height, outerRect.height)
                    y: outerRect.y + (outerRect.height - innerRect.height)
                    width: barWidth
                    color: "#485441"
                    radius: 2
                }
            }

            Item {
                Layout.fillHeight: true

                ColumnLayout {
                   anchors.fill: parent

                   spacing: 4

                   CText {
                       id: maxValueText
                       Layout.alignment: Qt.AlignTop

                       vAlign: Text.AlignTop
                       label: maxValue
                   }

                   CText {
                       vAlign: Text.AlignVCenter
                       label: currentValue
                   }

                   CText {
                       Layout.alignment: Qt.AlignBottom

                       vAlign: Text.AlignBottom
                       label: minValue
                   }
                }
            }
        }

        CText {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            hAlign: Text.AlignHCenter
            label: root.label
            fontSize: 16
            visible: label.length > 0
        }
    }
}
