import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0


Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        RowLayout {
            Layout.topMargin: 64
            spacing: 16

            Repeater {
                model: mainController.getSlidersModel()

                ColumnLayout {
                    spacing: 8

                    CVerticalSlider {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter

                        current: model.value

                        onSliderMoved: (value) => {
                            mainController.onSliderMoved(model.id, value);
                        }
                    }

                    CText {
                       Layout.fillWidth: true

                       hAlign: Qt.AlignHCenter
                       vAlign: Qt.AlignVCenter
                       label: "Slider%1".arg(model.id)
                       fontSize: 16
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
                mainController.onSwitchOutputClicked();
            }
        }

        Item {
            Layout.bottomMargin: 32
            Layout.fillHeight: true
        }
    }

    Popup {
        id: settingsFloatingButtonOverlay

        margins: 0
        padding: 0
        modal: false
        dim: false
        focus: true
        closePolicy: Popup.NoAutoClose
        visible: true
        parent: Overlay.overlay

        x: 16
        y: parent.height - height - 16

        width: openSettingsButtonButton.width
        height: openSettingsButtonButton.height

        background: Rectangle {
            color: "transparent"
            border.width: 0
        }

        CIconButton {
            id: openSettingsButtonButton

            iconName: "qrc:///resources/sliders_icon.png"
            toolTipText: "Open settings"
            buttonSize: 32

            anchors.centerIn: parent

            onButtonClicked: {
                settingsFloatingButtonOverlay.visible = false;
                stack.push(settingsView);
            }
        }
    }

    Component {
        id: settingsView
        SettingsView {}
    }
}
