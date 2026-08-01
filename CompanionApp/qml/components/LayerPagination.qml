pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Controls

Item {
    id: pagination

    signal pageChanged(page: int)

    readonly property int componentHeight: 28
    property int pageCount: 1
    property int currentPage: 0

    height: componentHeight

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillWidth: true
        }

        PrevNextButton {
            Layout.rightMargin: 12
            isNextButton: false
            enabled: pagination.currentPage > 0

            onButtonClicked: {
                if (pagination.currentPage > 0) {
                    pagination.pageChanged(--pagination.currentPage);
                }
            }
        }

        Repeater {
            id: pageRepeater
            model: pagination.pageCount

            Button {
                id: button

                required property int index
                property int buttonSize: pagination.componentHeight

                anchors.margins: 0
                padding: 0
                text: index + 1
                enabled: index != pagination.currentPage
                font.pointSize: 12

                contentItem: Item {
                    anchors.fill: parent
                    anchors.margins: 0

                    Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        anchors.centerIn: parent

                        text: button.text
                        font: button.font
                        color: button.getTextColor()
                    }
                }

                background: Rectangle {
                    width: button.buttonSize
                    height: button.buttonSize
                    implicitWidth: button.buttonSize
                    implicitHeight: button.buttonSize
                    color: button.getBgColor()
                    radius: 0

                    property int leftRadius: button.index == 0 ? height / 2 : 0
                    property int rightRadius: button.index == pagination.pageCount - 1  ? height / 2 : 0

                    topLeftRadius: leftRadius
                    bottomLeftRadius: leftRadius
                    topRightRadius: rightRadius
                    bottomRightRadius: rightRadius
                }

                onClicked: {
                    if (button.index !== pagination.currentPage) {
                        pagination.currentPage = button.index;
                        pagination.pageChanged(pagination.currentPage);
                    }
                }

                function getBgColor() {
                    if (!button.enabled) return Theme.buttonSecondaryPressed;
                    if (button.down) return Theme.buttonSecondaryPressed;
                    if (button.hovered) return Theme.buttonSecondaryHovered;
                    return Theme.backgroundSecondary;
                }

                function getTextColor() {
                    if (!button.enabled) return Theme.textPrimary;
                    return Theme.textSecondary;
                }
            }
        }

        PrevNextButton {
            Layout.leftMargin: 12
            isNextButton: true
            enabled: pagination.currentPage < pagination.pageCount - 1

            onButtonClicked: {
                if (pagination.currentPage < pagination.pageCount - 1) {
                    pagination.pageChanged(++pagination.currentPage);
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }

    component PrevNextButton : Button {
        id: iconButton

        signal buttonClicked

        property bool isNextButton: true
        property int buttonSize: pagination.componentHeight
        property int iconPadding: 8

        CIcon {
            id: icon

            anchors.centerIn: parent
            source: "qrc:///resources/icons/chevron_arrow.svg"
            iconSize: iconButton.buttonSize - iconButton.iconPadding * 2
            color: iconButton.getIconColor()
            flipIcon: iconButton.isNextButton ? false : true
        }

        background: Rectangle {
            width: iconButton.buttonSize
            height: iconButton.buttonSize
            implicitWidth: iconButton.buttonSize
            implicitHeight: iconButton.buttonSize
            color: iconButton.getBgColor()
            border.width: 0
            radius: height / 2
        }

        onClicked: iconButton.buttonClicked()

        ToolTip {
            id: tooltip
            text: iconButton.isNextButton ? qsTr("Next Layer") : qsTr("Previous layer")
            visible: iconButton.hovered && iconButton.enabled && !iconButton.down && text.length > 0
            delay: 600
        }

        function getBgColor() {
            if (!iconButton.enabled) return Theme.backgroundSecondary;
            if(iconButton.down) return Theme.buttonSecondaryPressed;
            if(iconButton.hovered) return Theme.buttonSecondaryHovered;
            return Theme.backgroundSecondary;
        }

        function getIconColor() {
            if (!iconButton.enabled) return Theme.textDisabled;
            if (iconButton.hovered || iconButton.down) return Theme.textPrimary;
            return Theme.textSecondary;
        }
    }
}
