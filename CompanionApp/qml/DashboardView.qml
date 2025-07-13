import QtQuick 2.15
import QtQuick.Layouts

import Controls 1.0
import "."

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: navigationBar.implicitWidth

            color: Theme.backgroundSecondary

            NavigationBar {
                id: navigationBar
                anchors.fill: parent
                tabButtonsModel: navBarModel

                onNavTabButtonClicked: (tabName) => {
                    label.text = tabName;
                }

                ListModel {
                    id: navBarModel

                    ListElement { type: "button"; name: qsTr("Keypad"); iconSource: "qrc:///resources/keys_icon.svg" }
                    ListElement { type: "button"; name: qsTr("Sliders"); iconSource: "qrc:///resources/sliders_icon.svg" }
                    ListElement { type: "spacer" }
                    ListElement { type: "button"; name: qsTr("Settings"); iconSource: "qrc:///resources/settings_icon.svg" }
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: Theme.backgroundPrimary

            CText {
                id: label
                anchors.centerIn: parent

                fontSize: 24
                hAlign: Text.AlignLeft
                color: Theme.textPrimary
            }
        }
    }
}
