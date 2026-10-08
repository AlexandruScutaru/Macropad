pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts

import Components


CVerticalScrollView {
    id: root

    required property SettingsController controller

    ColumnLayout {
        id: layoutId

        property int scrollBarOffset: 20
        property int maximumWidth: 600

        // trying to limit the size and center the contents, not the cleanest way though
        width: Math.min(root.width - scrollBarOffset, maximumWidth)
        x: (root.width - (width + scrollBarOffset)) / 2

        spacing: 12

        CText {
            Layout.fillWidth: true
            Layout.topMargin: 20

            label: qsTr("Theme")
            fontSize: 16
            fontWeight: Font.DemiBold

            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
        }

        CText {
            Layout.fillWidth: true

            label: qsTr("Change the look and feel of the aplication.")
            fontSize: 16
            fontWeight: Font.Normal

            hAlign: Text.AlignLeft
            vAlign: Text.AlignVCenter
            wrap: Text.Wrap
        }

        Flow {
            Layout.fillWidth: true
            Layout.topMargin: 8

            spacing: 20

            Repeater {
                id: themeSelectorsRepeater
                model: root.controller.themesListModel

                ThemeSelector {
                    Layout.alignment: Qt.AlignHCenter

                    required property string name
                    required property variant colors

                    model: colors

                    toolTipText: name
                    selected: name === root.controller.currentTheme

                    onThemeSelected: {
                        if (name != root.controller.currentTheme) {
                            root.controller.changeTheme(name);
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
