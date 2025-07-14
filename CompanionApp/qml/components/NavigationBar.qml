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
    property bool expanded: false

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

    PropertyAnimation {
        id: expandAnimation

        target: navBar
        property: "implicitWidth"
        to: navBar.expanded ? navBar.collapsed_width : navBar.expanded_width
        duration: 200
        easing.type: Easing.InQuad

        onFinished: {
            navBar.expanded = !navBar.expanded;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: navBar.margin

        CIconButton {
            Layout.bottomMargin: 8
            Layout.alignment: expandAnimation.running || navBar.expanded ? Qt.AlignRight : Qt.AlignHCenter

            iconName: "qrc:///resources/expand_icon.svg"
            toolTipText: navBar.expanded ? qsTr("Collpase") : qsTr("Expand")
            iconAnimationType: CIcon.AnimationType.Rotate
            iconToggleAnimation: true
            iconFlipIcon: navBar.expanded

            onButtonClicked: {
                expandAnimation.running = true;
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
            required property int animation

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            label: name
            toolTipText: name
            iconName: iconSource
            checked: name === navBar.currentSelection
            expanded: !expandAnimation.running && navBar.expanded
            iconAnimationType: animation

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
