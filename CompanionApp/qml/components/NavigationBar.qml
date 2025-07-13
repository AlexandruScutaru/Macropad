pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Layouts
import QtQml.Models

import Controls 1.0

Item {
    id: navBar

    signal navTabButtonClicked(name: string)

    readonly property int expanded_width: 160
    readonly property int collapsed_width: 56
    readonly property int margin: 8

    property ListModel tabButtonsModel
    property string currentSelection
    property bool expanded: true

    implicitWidth: expanded ? expanded_width : collapsed_width

    function tabSelected (tabName: string) {
        navBar.currentSelection = tabName;
        navBar.navTabButtonClicked(tabName);
    }

    Component.onCompleted: {
        if (tabButtonsModel.count == 0) {
            return;
        }

        navBar.tabSelected(tabButtonsModel.get(0).name);
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: navBar.margin

        CIconButton {
            Layout.bottomMargin: 8
            Layout.alignment: navBar.expanded ? Qt.AlignRight : Qt.AlignHCenter

            iconName: "qrc:///resources/%1.svg".arg(navBar.expanded ? "collapse_icon" : "expand_icon")
            toolTipText: navBar.expanded ? qsTr("Collpase") : qsTr("Expand")
            onButtonClicked: {
                navBar.expanded = !navBar.expanded;
            }
        }

        Repeater {
            id: tabButtonsRepeater
            model: navBar.tabButtonsModel
            delegate: chooser

            DelegateChooser {
                id: chooser
                role: "type"

                DelegateChoice { roleValue: "button"; delegate: tabButton }
                DelegateChoice { roleValue: "spacer"; delegate: tabSpacer }
            }
        }
    }

    Component {
        id: tabButton

        CTabButton {
            required property string name
            required property string iconSource

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            label: name
            toolTipText: name
            iconName: iconSource
            checked: name === navBar.currentSelection
            expanded: navBar.expanded

            onButtonClicked: {
                navBar.tabSelected(label);
            }
        }
    }

    Component {
        id: tabSpacer

        Item {
            Layout.fillHeight: true
        }
    }
}
