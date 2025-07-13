import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: progressBar

    property int minValue: 0
    property int maxValue: 100
    property int currentValue: 50
    property int barWidth: 4

    // really not sure about this. will have to revisit this one at a later time
    width: barWidth + rowLayout.spacing + maxValueText.width

    RowLayout {
        id: rowLayout

        anchors.fill: parent
        spacing: 6

        Rectangle {
            id: outerRect

            Layout.fillHeight: true
            Layout.preferredWidth: progressBar.barWidth
            Layout.alignment: Qt.AlignRight

            color: Theme.textPrimary
            radius: 2

            Rectangle {
                id: innerRect

                function getHeight() {
                    const delta = progressBar.currentValue - progressBar.minValue;
                    const range = progressBar.maxValue - progressBar.minValue;
                    return Math.min((delta / range) * outerRect.height, outerRect.height);
                }

                height: innerRect.getHeight();
                y: outerRect.y + (outerRect.height - innerRect.height)
                width: outerRect.width
                color: Theme.accentPrimaryNormal
                radius: 2
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: maxValueText.width

            ColumnLayout {
                anchors.fill: parent
                spacing: 4

                CText {
                    id: maxValueText
                    Layout.alignment: Qt.AlignTop

                    vAlign: Text.AlignTop
                    label: progressBar.maxValue
                }

                CText {
                    vAlign: Text.AlignVCenter
                    label: progressBar.currentValue
                }

                CText {
                    Layout.alignment: Qt.AlignBottom

                    vAlign: Text.AlignBottom
                    label: progressBar.minValue
                }
            }
        }
    }
}
