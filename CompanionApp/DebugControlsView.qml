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
                required property int index
                spacing: 8

                CText {
                   Layout.fillWidth: true
                   Layout.preferredWidth: 2
                   label: "Slider" + " " + index
                   fontSize: 18
                }

                CSlider {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 3

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
