import QtQuick
import QtQuick.Layouts

import Controls

Item {
    id: keyconfig

    property var model

    CText {
        id: configUnavailable
        anchors.centerIn: parent

        label: qsTr("Select a key to configure it")
        fontSize: 12
        hAlign: Text.AlignHCenter | Text.AlignVCenter
        color: Theme.textPrimary

        visible: keyconfig.model.keyActionId === "" || keyconfig.model.keyActionId === undefined
    }

    Item {
        anchors.fill: parent
        visible: !configUnavailable.visible

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 12
            anchors.bottomMargin: 12

            spacing: 8

            CText {
                Layout.fillWidth: true
                Layout.leftMargin: 20
                Layout.rightMargin: 20

                label: keyconfig.model.keyActionName ?? ""
                fontSize: 14
                hAlign: Text.AlignLeft
                color: Theme.textPrimary
            }

            GradientSeparator {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.rightMargin: 8

                colorEnds: "transparent"
                colorMiddle: Theme.textDisabled
                orientation: Gradient.Horizontal
            }

            CVerticalScrollView {
                id: scrollView

                Layout.fillWidth: true
                Layout.fillHeight: true

                Repeater {
                    id: optionsRepeater

                    model: keyconfig.model.keyActionConfig

                    Item {
                        id: option

                        anchors.fill: parent

                        anchors.leftMargin: 20
                        anchors.rightMargin: 28

                        required property string name
                        required property int type

                        RowLayout {
                            spacing: 8

                            CText {
                                label: option.name
                                fontSize: 12
                                hAlign: Text.AlignLeft
                                color: Theme.textPrimary
                            }

                            CText {
                                label: option.type === Keypad.String ? "string" : "other"
                                fontSize: 12
                                hAlign: Text.AlignLeft
                                color: Theme.textPrimary
                            }
                        }
                    }
                }
            }
        }
    }
}
