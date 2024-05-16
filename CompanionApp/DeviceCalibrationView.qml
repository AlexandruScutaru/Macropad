import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.alignment: Qt.AlignTop

            label: qsTr("Calibration view")
            fontSize: 20
            hAlign: Text.AlignHCenter
        }

        RowLayout {
            spacing: 8

            Repeater {
                model: controller.getDeviceCalibrationModel()

                CVerticalProgressBar {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    required property int id
                    required property int min
                    required property int max
                    required property int value

                    minValue: min
                    maxValue: max
                    currentValue: value
                    label: qsTr("Slider") + id
                }
            }
        }

        CButton {
            id: finishButton
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 16

            label: qsTr("Finish")
            toolTipText: qsTr("Finish calibration")

            onButtonClicked: {
                controller.setIsCalibrating(false);
                stack.pop();
            }
        }
    }
}
