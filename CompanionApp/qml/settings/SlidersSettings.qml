import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0
import MacropadCompanion 1.0


Item {
    SlidersSettingsController {
        id:  slidersController
    }

    Component.onCompleted: {
        slidersController.init(controller.getConfig());
    }

    Connections {
        target: controller
        function onPotentiometersChanged(values) {
            slidersController.onPotentiometersChanged(values)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            Layout.topMargin: 16
            spacing: 8

            Repeater {
                model: slidersController.getModel()

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
            id: calibrationButton
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 8

            label: slidersController.isCalibrating ? qsTr("Finish") : qsTr("Calibrate")
            toolTipText: slidersController.isCalibrating ? qsTr("Finish calibration") : qsTr("Start calibration")

            onButtonClicked: {
                slidersController.setIsCalibrating(!slidersController.isCalibrating);
                controller.setIsCalibrating(slidersController.isCalibrating);
            }
        }
    }
}
