import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true;
        }

        CText {
            Layout.fillWidth: true

            label: qsTr("Device connected")
            fontSize: 28
            hAlign: Text.AlignHCenter
        }

        CButton {
            Layout.alignment: Qt.AlignHCenter

            label: "Calibrate"
            toolTipText: "Calibrate the sliders"

            onButtonClicked: {
                controller.setIsCalibrating(true);
                stack.push(calibrationView)
            }
        }

        Item {
            Layout.fillHeight: true;
        }
    }

    Component {
        id: calibrationView
        DeviceCalibrationView {}
    }
}
