pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQml.Models

import Controls

Item {
    id: navBar

    signal navTabButtonClicked(name: string)
    signal navBarExpandedChanged(expanded: bool)

    readonly property int expanded_width: 180
    readonly property int collapsed_width: 56
    readonly property int margin: 8

    property ListModel tabButtonsModel
    property string currentSelection
    property bool expanded: true

    implicitWidth: expanded ? expanded_width : collapsed_width

    Component.onCompleted: {
        if (tabButtonsModel.count == 0) {
            return;
        }

        const tabName = tabButtonsModel.get(0).name;
        navBar.currentSelection = tabName;
        navBar.navTabButtonClicked(tabName);
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
            navBar.navBarExpandedChanged(navBar.expanded);
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: navBar.margin

        CIconButton {
            Layout.bottomMargin: 8
            Layout.alignment: expandAnimation.running || navBar.expanded ? Qt.AlignRight | Qt.AlignTop : Qt.AlignHCenter | Qt.AlignTop

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
            required property bool tabEnabled
            required property bool tabCheckable
            required property string iconSource
            required property int animation

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            label: name
            toolTipText: name
            iconName: iconSource
            checked: tabCheckable && name === navBar.currentSelection
            expanded: !expandAnimation.running && navBar.expanded
            iconAnimationType: animation
            enabled: tabEnabled

            onButtonClicked: {
                if (tabCheckable) {
                    navBar.currentSelection = label;
                }
                navBar.navTabButtonClicked(label);
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
