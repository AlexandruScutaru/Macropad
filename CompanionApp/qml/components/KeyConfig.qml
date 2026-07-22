pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQml.Models

import Controls

Item {
    id: keyconfig

    required property ActionConfigController controller

    CText {
        id: configUnavailable
        anchors.centerIn: parent

        label: qsTr("Select a key to configure it")
        fontSize: 14
        hAlign: Text.AlignHCenter | Text.AlignVCenter
        color: Theme.textPrimary

        visible: keyconfig.controller.keyActionId === "" || keyconfig.controller.keyActionId === undefined
    }

    Item {
        anchors.fill: parent
        visible: !configUnavailable.visible

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 12
            anchors.bottomMargin: 12

            spacing: 12

            CText {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                label: keyconfig.controller.keyActionDisplayName
                fontSize: 14
                hAlign: Text.AlignLeft
                color: Theme.textPrimary
            }

            GradientSeparator {
                Layout.fillWidth: true
                Layout.preferredHeight: 1

                colorEnds: "transparent"
                colorMiddle: Theme.textDisabled
                orientation: Gradient.Horizontal
            }

            CText {
                id: noOptions
                Layout.fillWidth: true

                label: qsTr("Action has no options")
                fontSize: 12
                hAlign: Text.AlignHCenter
                color: Theme.textPrimary

                visible: keyconfig.controller.model.count === 0
            }

            Item {
                visible: noOptions.visible
                Layout.fillHeight: true
            }

            CVerticalScrollView {
                id: scrollView

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 4

                visible: !noOptions.visible

                ColumnLayout {
                    width: scrollView.width
                    spacing: 8

                    Repeater {
                        id: optionsRepeater

                        model: keyconfig.controller.model
                        delegate: chooser

                        DelegateChooser {
                            id: chooser
                            role: "type"

                            DelegateChoice { roleValue: Keypad.String; delegate: stringOption }
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }

    Component {
        id: stringOption

        Item {
            id: option

            Layout.fillWidth: true
            Layout.preferredHeight: row.height
            Layout.leftMargin: 20
            Layout.rightMargin: 20

            required property string name
            required property string displayName
            required property string tooltip
            required property string value

            RowLayout {
                id: row
                width: option.width
                spacing: 8

                CText {
                    Layout.fillWidth: true
                    label: option.displayName
                    fontSize: 12
                    hAlign: Text.AlignLeft
                    color: Theme.textPrimary
                }

                CTextField {
                    Layout.preferredWidth: option.width * 0.66

                    text: option.value
                    toolTipText: option.tooltip
                    radius: 8

                    onInputAccepted: (value) => {
                        keyconfig.controller.optionChanged(option.name, value);
                    }
                }
            }
        }
    }
}
