import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Button {
    id: button

    activeFocusOnTab: true
    focusPolicy: Qt.StrongFocus

    signal buttonClicked

    property alias expanded: chevronIcon.expanded

    property int buttonHeight: 50
    property int marginSize: 12
    property int iconSize: 20
    property string iconName
    property string label
    property string toolTipText
    property int fontSize: 14

    font.pointSize: fontSize
    text: label
    anchors.margins: 0
    padding: 0

    contentItem: Item {
        anchors.fill: parent
        anchors.margins: 0

        RowLayout {
            anchors.fill: parent
            spacing: button.marginSize
            anchors.leftMargin: button.marginSize
            anchors.rightMargin: button.marginSize

            CIcon {
                id: chevronIcon

                property bool expanded: false
                Layout.preferredWidth:  16
                Layout.preferredHeight: 16
                Layout.alignment: Qt.AlignLeft

                source: "qrc:///resources/icons/chevron_arrow.svg"
                iconSize: 16
                color: button.getTextColor()

                rotation: expanded ? 90 : 0
                Behavior on rotation {
                    NumberAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 200
                    }
                }
            }

            CIcon {
                id: icon

                Layout.preferredWidth: button.iconSize
                Layout.preferredHeight: button.iconSize
                Layout.alignment: Qt.AlignLeft

                source: button.iconName
                iconSize: button.iconSize
                color: button.getTextColor()
                visible: button.iconName.length > 0
            }

            Text {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                Layout.topMargin: -1

                text: button.text
                font: button.font
                color: button.getTextColor()
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }

    background: Rectangle {
        id: buttonBackground

        implicitWidth: 80
        implicitHeight: button.buttonHeight
        color: button.getBgColor()
        border.width: 0
        radius: 0
    }

    CFocusOutline {
        target: button
        anchors.fill: button
        radius: 0
    }

    onClicked: {
        chevronIcon.expanded = !chevronIcon.expanded;
        button.buttonClicked();
    }

    function getBgColor() {
        if (button.down) return Theme.buttonSecondaryPressed;
        if (button.hovered) return Theme.buttonSecondaryHovered;
        return Theme.buttonSecondaryNormal;
    }

    function getTextColor() {
        if (button.hovered) return Theme.textPrimary;
        return Theme.textSecondary;
    }

    ToolTip {
        id: tooltip
        text: button.toolTipText
        visible: button.hovered && button.enabled && !button.down && text.length > 0
        delay: 600
    }
}
