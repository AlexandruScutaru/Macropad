import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15


Item {
    id: root
    ColumnLayout {
        id: mainLayout

        width: root.width
        height: root.height
        spacing: 12

        Item {
            Layout.fillHeight: true
        }

        Repeater {
            model: 4

            RowLayout {
                id: row
                required property int index
                spacing: 8

                CText {
                   Layout.fillWidth: true
                   Layout.maximumWidth: row.width * 0.3
                   label: "Slider" + " " + index
                   fontSize: 16
                }

                CSlider {
                    Layout.fillWidth: true

                    current: 50
                }
            }
        }

        CButton {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 20

            label: "Switch Output"
            toolTipText: "Cycle through available audio output devices"

            onButtonClicked: {
                console.log("Switch Output button clicked");
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
