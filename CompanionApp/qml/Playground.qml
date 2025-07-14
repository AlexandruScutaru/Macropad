import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic 2.15

import Controls 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: Theme.backgroundPrimary

    Item {
        anchors.fill : parent
        anchors.margins: 20
        anchors.rightMargin: 4

        ScrollView {
            id: scrollView
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            anchors.fill: parent

            ColumnLayout {
                width: scrollView.width - 20
                spacing: 12

                CText {
                    Layout.fillWidth: true

                    label: "This is a primary label"
                    fontSize: 24
                    hAlign: Text.AlignLeft
                    color: Theme.textPrimary
                }

                CText {
                    Layout.fillWidth: true

                    label: "This is a secondary label"
                    fontSize: 16
                    hAlign: Text.AlignLeft
                    color: Theme.textSecondary
                }

                CText {
                    Layout.fillWidth: true

                    label: "This is a label for disabled controls"
                    fontSize: 16
                    hAlign: Text.AlignLeft
                    color: Theme.textDisabled
                }

                CText {
                    Layout.fillWidth: true

                    label: "This is a a very long string that should elide, in which case there will be a tooltip showing the entire contents"
                    fontSize: 16
                    hAlign: Text.AlignLeft
                    color: Theme.textPrimary
                }

                CText {
                    Layout.fillWidth: true

                    label: "This is a a very long string that should wrap to the next line to be fully visible at any time."
                    fontSize: 16
                    hAlign: Text.AlignLeft
                    color: Theme.textPrimary
                    wrap: Text.Wrap
                }

                Item {
                    Layout.fillWidth: true
                    implicitHeight: 38

                    RowLayout {
                        anchors.fill: parent
                        spacing: 12

                        CButton {
                            label: "Enabled"
                            toolTipText: "This is button"
                        }

                        CButton {
                            label: "Disabled"
                            enabled: false
                        }

                        CIconButton {
                            iconName: "qrc:///resources/expand_icon.svg"
                            iconAnimationType: CIcon.AnimationType.Rotate
                            iconToggleAnimation: true
                        }

                        CIconButton {
                            iconName: "qrc:///resources/settings_icon.svg"
                            iconAnimationType: CIcon.AnimationType.Rotate
                        }

                        CIconButton {
                            iconName: "qrc:///resources/sliders_icon.svg"
                            iconAnimationType: CIcon.AnimationType.Scale
                        }

                        CIconButton {
                            iconName: "qrc:///resources/keys_icon.svg"
                        }

                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }

                CTabButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    label: "Tab button enabled"
                    checked: false

                    onButtonClicked: {
                        checked = !checked;
                    }
                }

                CTabButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    label: "Other tab button"
                    checked: false

                    onButtonClicked: {
                        checked = !checked;
                    }
                }

                CTabButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    label: "Tab button with animated icon"
                    iconName: "qrc:///resources/keys_icon.svg"
                    iconAnimationType: CIcon.AnimationType.Rotate
                }

                CTabButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    label: "Tab button with static icon"
                    iconName: "qrc:///resources/keys_icon.svg"
                }

                CTabButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    label: "Tab button disabled"
                    checked: false
                    enabled: false
                }


                Item {
                    id: sliderContainer

                    Layout.fillWidth: true
                    Layout.preferredHeight: 160

                    RowLayout {
                        anchors.fill: parent
                        spacing: 12
                        Layout.margins: 0

                        Item {
                            Layout.fillWidth: true
                        }

                        CVerticalSlider {
                            id: slider

                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignHCenter

                            current: 25
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 8

                                CVerticalProgressBar {
                                    Layout.fillHeight: true
                                    Layout.alignment: Qt.AlignHCenter

                                    minValue: 0
                                    maxValue: 100
                                    currentValue: slider.value
                                }

                                CText {
                                    Layout.fillWidth: true
                                    Layout.alignment: Qt.AlignHCenter

                                    hAlign: Text.AlignHCenter
                                    label: "Slider"
                                    fontSize: 16
                                    visible: label.length > 0
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }

                CSlider {
                    Layout.leftMargin: 30
                    Layout.rightMargin: 30
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    current: 25
                }

                CTextField {
                    Layout.leftMargin: 30
                    Layout.rightMargin: 30
                    Layout.fillWidth: true

                    validator: IntValidator { bottom: 100000; top: 999999}

                    placeholder: "Placeholder text"
                    toolTipText: "Enter a valid number in range [100000, 999999]"

                    onInputAccepted: (value) => {}
                }

                CBusyIndicator {
                    Layout.preferredWidth: 64
                    Layout.preferredHeight: 64
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }
}
