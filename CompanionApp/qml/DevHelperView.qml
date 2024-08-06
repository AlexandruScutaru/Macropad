import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Item {
    id: root

    anchors.fill: parent
    anchors.margins: 16

    required property DevHelperController controller

    ColumnLayout {
        id: mainLayout

        anchors.fill: root
        spacing: 12

        Item {
            Layout.fillHeight: true
        }

        Repeater {
            model: controller.getModel()

            RowLayout {
                spacing: 8

                CText {
                   Layout.fillWidth: true
                   Layout.minimumWidth: 70
                   label: "Slider%1".arg(model.id)
                   fontSize: 16
                }

                CSlider {
                    Layout.fillWidth: true
                    current: model.value

                    onSliderMoved: (value) => {
                        controller.onSliderMoved(model.id, value);
                    }
                }
            }
        }

        CButton {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20

            label: "Switch Output"
            toolTipText: "Cycle through available audio output devices"

            onButtonClicked: {
                controller.onSwitchOutputClicked();
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
